#include <iostream>
#include <boost/locale.hpp>

#include "csv.hpp"
#include "re2/re2.h"
#include "re2/stringpiece.h"
#include "emojis.h"

using namespace std;
using namespace csv;

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


inline void to_json(ostream &stream, std::string &text, std::string &raw, std::set<std::string> &emojis){
    stream << "{\"text\":\"" << text << "\",";
    stream << "\"raw\":\"" << raw << "\",";
    stream << "\"emojis\":[";

    int i = 0;
    for (std::string e: emojis){
        if (i > 0){
            stream << ",";
        }

        stream << "\"" << e << "\"";
        i++;
    }

    stream <<  "]}" << std::endl;
}


int main (int argc, char *argv[]) {
    CSVFormat format;
    format.delimiter('\t');  // Header is on 3rd row (zero-indexed)
    format.column_names({"c1", "text", "c2", "c3", "c4", "lang", "c6"});

    // setup locale
    boost::locale::generator gen;
    std::locale loc=gen(""); 
    std::locale::global(loc);

    CSVReader reader = CSVReader("./data/e_top1000.txt", format);

    for (CSVRow& row: reader) { // Input iterator
        std::string text_raw = row["text"].get<>();

        // unicode normalization
        std::string text =  boost::locale::normalize(text_raw,boost::locale::norm_nfd, loc);

        if ( prefilter(text) == false || row["lang"].get<>() != "en" ) {
            continue;
        }

        // extract emojis
        std::set<std::string> emojis;
        std::string emoji;

        re2::StringPiece input(text);

        while (RE2::FindAndConsume(&input, emoji_re, &emoji)) {
            emojis.insert(emoji);
        }

        // clean up: remove emojis, quotation marks, newline..
        text = clean(text);

        // pos tag filtering
        // INFO: not so easy in c++... shift it to python

        // write out
        to_json(std::cout ,text, text_raw, emojis);

    }

    exit(0);
}