#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include "JsonValue.hpp"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <algorithm>

class JsonParser {
public:
    JsonParser(const std::string& filepath);
    ~JsonParser() {};

    JsonValue parse(void);

private:
    std::string _content;
    std::string::iterator _itr;

    JsonValue parseValue(void);
    JsonValue parseNull(void);
    JsonValue parseString(void);
    JsonValue parseNumber(void);
    JsonValue parseBoolean(void);
    JsonValue parseArray(void);
    JsonValue parseObject(void);

    bool isEndOrDelimiter(void) {
        return *_itr == ',' || *_itr == '}' || *_itr == ']' || _itr == _content.end();
    }
};

/*------------------------------ERROR MESSAGES--------------------------------*/
static std::string ERR_OPEN("Parser: cannot open file");
static std::string ERR_EMPTY("Parser: file is empty");
static std::string ERR_NOT_REGULAR("Parser: not a regular file");
static std::string ERR_JSON_PARSE("Parser: invalid JSON syntax");
static std::string ERR_JSON_TYPE("Parser: invalid JSON type");
static std::string ERR_JSON_QUOTE("Parser: unclosed quote");

/*----------------------------------COLORS------------------------------------*/
#define RE "\001\033[0;39m\002" // reset
#define W  "\001\033[1;97m\002" // white
#define R  "\001\033[1;91m\002" // red
#define G  "\001\033[1;92m\002" // green light

#endif