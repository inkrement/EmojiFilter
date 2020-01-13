#include <iostream>
#include <fstream>
#include <boost/locale.hpp>
#include <pthread.h>
#include "re2/re2.h"
#include "re2/stringpiece.h"
#include "emojis.h"

using namespace std;

// regular expressions
RE2 url_re("https?://(?:[-\\w.]|(?:%[\\da-fA-F]{2}))+");
RE2 alpha("([^A-Za-z0-9\\.!, ])");
RE2 whitespace("(\\s+)");
RE2 num("\\s(\\d+)[^am|pm]|d+$");
RE2 mention("(@\\w+)");
RE2 emoji_re(emojiRegex());

std::string& clean(std::string &str){
    str = boost::locale::to_lower(str);

    // remove mentions
    RE2::GlobalReplace(&str, mention, "");

    // trim all whitespace to single
    RE2::GlobalReplace(&str, alpha, " ");

    // replace numbers
    RE2::GlobalReplace(&str, num, " <NUM> ");

    // trim all whitespace to single
    RE2::GlobalReplace(&str, whitespace, " ");

    //trim trailing space
    str.erase(0, str.find_first_not_of("\t\n\v\f\r "));
    str.erase(str.find_last_not_of("\t\n\v\f\r ") + 1);

    return str;
}


inline bool prefilter(std::string &str){
    // skip retweets
    if (str.rfind("RT ", 0) == 0)
        return false;

    // skip replies
    if (str.rfind("@", 0) == 0)
        return false;
    
    // too short
    if (str.length() < 3)
        return false;

    // ignore if there is an url
    if (RE2::PartialMatch(str, url_re))
        return false;

    // check if there is an emoji
    if (!RE2::PartialMatch(str, emoji_re))
        return false;

    return true;
}


std::string ReplaceAll(std::string &str, const char& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, 1, to);
        start_pos += to.length();
    }
    return str;
}


std::string escapeJSON(std::string &raw) {
    ReplaceAll(raw, '\\', "\\\\");
    ReplaceAll(raw, '\n', "\\n");
    ReplaceAll(raw, '\f', "\\f");
    ReplaceAll(raw, '\r', "\\r");
    ReplaceAll(raw, '\t', "\\t");
    ReplaceAll(raw, '\b', "\\b");
    ReplaceAll(raw, '"', "\\\"");

    return raw;
}


inline void to_json(ostream &stream, std::string &text, std::string &raw, std::set<std::string> &emojis){
    stream << "{\"text\":\"" << text << "\",";
    stream << "\"raw\":\"" << escapeJSON(raw) << "\",";
    stream << "\"emojis\":[";

    int i = 0;
    for (std::string e: emojis){
        if (i > 0){
            stream << ",";
        }

        stream << "\"" << e << "\"";
        i++;
    }

    stream <<  "]}";
}

std::optional<std::string> nextLine(std::istream &input) {
    // TSV (mysql dump format)
    // "c1", "text", "c2", "c3", "c4", "lang", "c6"
    // skip 

    bool escaped = false;

    char ch;
    int field = 0;
    
    // {0: not parsed yet 1: first character e 2: english -1: not english}
    int lang = 0;

    std::stringstream output;

    while (input >> noskipws >> ch) {
        if (ch == '\\'){
            escaped = true;
            continue;
        }

        if (ch == '\t' && !escaped){
            field++;
            continue;
        }

        if (ch == '\n' && !escaped){
            field=0;

            // break
            std::string ret = output.str();
            output.clear();

            if (lang == 2) {
                return ret;
            }
            // reset and parse next line
            field = 0;
            lang = 0;
            escaped = false;
            continue;
        }

        if (field == 1){
            // if escaped && N null value (!)
            if (ch == 'N' && escaped) {
                return {};
            }
            output << ch;
        }

        //language
        if (field == 5){
            if (lang == 0 && ch == 'e') {
                lang = 1;
            } else if (lang == 1 && ch == 'n') {
                lang = 2;
            } else {
                lang = -1;
            }
        }

        escaped = false;
    }

    return {};
}


int main (int argc, char *argv[]) {
    if (argc != 2) {
	    std::cerr << "USAGE: emoji_filter input_file.txt" << std::endl;
    }

    // setup locale
    boost::locale::generator gen;
    std::locale loc=gen(""); 
    std::locale::global(loc);

    std::ifstream tsv = std::ifstream(argv[1]);

    int line = 0;

    while (tsv.peek() != EOF){
        std::optional<std::string> text_raw = nextLine(tsv);

        if (text_raw) {
            // text processing 
            //cout << text_raw.value() << endl;

            if (text_raw.value().length() == 0){
                continue;
            }

            // unicode normalization
            std::string text =  boost::locale::normalize(text_raw.value(), boost::locale::norm_nfd, loc);
	        //std::string text = text_raw;

            if ( prefilter(text) == false) {
                continue;
            }

            // extract emojis
            std::set<std::string> emojis;
            std::string emoji;

	        //std::cerr << text << endl;
            re2::StringPiece input(text);

	        //cout << input.length() << endl;
            while (RE2::FindAndConsume(&input, emoji_re, &emoji)) {
                emojis.insert(emoji);
            }

            //emojis.insert("test");

            // clean up: remove emojis, quotation marks, newline..
            text = clean(text);

            // pos tag filtering
            // INFO: not so easy in c++... shift it to python

            // write out
            if (line > 0) { std::cout << endl; }
            to_json(std::cout ,text, text_raw.value(), emojis);

            line++;
        }
    }

    exit(0);
}
