#pragma once

#include <string>
// smileys & emotions

std::vector<std::string> blacklist = {
    // Zero Width Joiner 
    "\U0000200D",

    // Combining Diacritical Marks for Symbols
    // (i.e. Combining Enclosing Keycap or COMBINING ENCLOSING SCREEN)
    "\U000020D0","\U000020D1","\U000020D2","\U000020D3","\U000020D4",
    "\U000020D5","\U000020D6","\U000020D7","\U000020D8","\U000020D9",
    "\U000020DA","\U000020DB","\U000020DC","\U000020DD","\U000020DE",
    "\U000020DF",
    "\U000020E0","\U000020E1","\U000020E2","\U000020E3","\U000020E4",
    "\U000020E5","\U000020E6","\U000020E7","\U000020E8","\U000020E9",
    "\U000020EA","\U000020EB","\U000020EC","\U000020ED","\U000020EE",
    "\U000020EF",
    "\U000020F0","\U000020FF","\U000020F2","\U000020F3","\U000020F4",
    "\U000020F5","\U000020F6","\U000020F7","\U000020F8","\U000020F9",
    "\U000020FA","\U000020FB","\U000020FC","\U000020FD","\U000020FE",
    "\U000020FF"
};

std::vector<std::string> sm_emojis = {
        // face smiling
        "\U0001F600","\U0001F601","\U0001F602","\U0001F603","\U0001F604",
        "\U0001F605","\U0001F606","\U0001F607","\U0001F609","\U0001F60A",
        "\U0001F642","\U0001F643","\U0001F923",
        // face affection
        "\U0001F970","\U0001F60D","\U0001F929","\U0001F618","\U0001F617",
        "\U0000263A","\U0001F61A","\U0001F619",
        // face tongue
        "\U0001F60B","\U0001F61B","\U0001F61C","\U0001F92A","\U0001F61D",
        "\U0001F911",
        // face hand
        "\U0001F917","\U0001F92D","\U0001F92B","\U0001F914",
        // face neutral sceptical
        "\U0001F910","\U0001F928","\U0001F610","\U0001F611","\U0001F636",
        "\U0001F60F","\U0001F612","\U0001F644","\U0001F62C","\U0001F925",
        // face sleepy
        "\U0001F60C","\U0001F614","\U0001F62A","\U0001F924","\U0001F634",
        // face unwell
        "\U0001F637","\U0001F912","\U0001F915","\U0001F922","\U0001F92E",
        "\U0001F927","\U0001F975","\U0001F976","\U0001F974","\U0001F635",
        "\U0001F92F",
        // face hat
        "\U0001F920","\U0001F973",
        // face glasses
        "\U0001F60E","\U0001F913","\U0001F9D0",
        // face concerned
        "\U0001F615","\U0001F61F","\U0001F641","\U00002639","\U0001F62E",
        "\U0001F62F","\U0001F632","\U0001F633","\U0001F97A","\U0001F626",
        "\U0001F627","\U0001F628","\U0001F630","\U0001F625","\U0001F622",
        "\U0001F62D","\U0001F631","\U0001F616","\U0001F623","\U0001F61E",
        "\U0001F613","\U0001F629","\U0001F62B","\U0001F971",
        // face negative
        "\U0001F624","\U0001F621","\U0001F620","\U0001F92C","\U0001F608",
        "\U0001F47F","\U0001F480","\U00002620",
        // face costume
        "\U0001F4A9","\U0001F921","\U0001F479","\U0001F47A","\U0001F47B",
        "\U0001F47D","\U0001F47E","\U0001F916",
        // cat face
        "\U0001F63A","\U0001F638","\U0001F639","\U0001F63B","\U0001F63C",
        "\U0001F63D","\U0001F640","\U0001F63F","\U0001F63E",
        // monkey face
        "\U0001F648","\U0001F649","\U0001F64A",
        // emotion
        "\U0001F48B","\U0001F48C","\U0001F498","\U0001F49D","\U0001F496",
        "\U0001F497","\U0001F493","\U0001F49E","\U0001F495","\U0001F49F",
        "\U00002763","\U0001F494","\U00002764","\U0001F9E1","\U0001F49B",
        "\U0001F49A","\U0001F499","\U0001F49C","\U0001F90E","\U0001F5A4",
        "\U0001F90D","\U0001F4AF","\U0001F4AF","\U0001F4A2","\U0001F4A5",
        "\U0001F4AB","\U0001F4A6","\U0001F4A8","\U0001F573","\U0001F4A3",
        "\U0001F4AC",
        // U+1F441 U+FE0F U+200D U+1F5E8 U+FE0F
        "\U0001F441\U0000FE0F\U0000200D\U0000FE0F",
        "\U0001F5E8","\U0001F5EF","\U0001F4AD","\U0001F4A4"
    };

inline std::string blacklistOR(){
    std::string regex = "";

    for (std::string &e: blacklist){
        if (&e != &blacklist.front()){
            regex += "|";
        }

        regex += e;
    }
    return regex;
}

inline std::string emojiOR(){
    std::string regex = "";

    for (std::string &e: sm_emojis){
        if (&e != &sm_emojis.front()){
            regex += "|";
        }

        regex += e;
    }

    return regex;
}

// TODO: allow also nothing as preceeding character
inline std::string emojiRegex(){
    // TODO: avoid U+200D (U+FE0F?)
    return "(" + emojiOR() + ")";
    //return "[" + emojiOR() + "]{1}[^" + blacklistOR() + "]";
}


int codepoint(const std::string &u)
{
    int l = u.length();
    if (l<1) return -1; unsigned char u0 = u[0]; if (u0>=0   && u0<=127) return u0;
    if (l<2) return -1; unsigned char u1 = u[1]; if (u0>=192 && u0<=223) return (u0-192)*64 + (u1-128);
    if (l<3) return -1; unsigned char u2 = u[2]; if (u0>=224 && u0<=239) return (u0-224)*4096 + (u1-128)*64 + (u2-128);
    if (l<4) return -1; unsigned char u3 = u[3]; if (u0>=240 && u0<=247) return (u0-240)*262144 + (u1-128)*4096 + (u2-128)*64 + (u3-128);
    return -1;
}
 
std::string codepointhex(const std::string &u)
{
    std::stringstream ss;
    std::string s;
    ss << std::showbase << std::hex << codepoint(u);
    ss >> s;
    return s;
}