#include "JsonParser.hpp"

JsonParser::JsonParser(const std::string& filepath) {
    std::cout << "Reading config file: " << W << filepath << RE << std::endl;

    // Checking if the file exists and is a regular file
    struct stat filestat;
    if (stat(filepath.c_str(), &filestat) != 0)
        throw std::runtime_error(ERR_OPEN);
    if (!S_ISREG(filestat.st_mode))
        throw std::runtime_error(ERR_NOT_REGULAR);

    // Checking if the file is a JSON file
    if (filepath.substr(filepath.find_last_of(".") + 1) != "json")
        throw std::runtime_error(ERR_JSON_PARSE);

    // Opening the file and checking if it's empty
    std::ifstream inputFileStream(filepath.c_str());
    if (inputFileStream.fail())
        throw std::runtime_error(ERR_OPEN);
    if (inputFileStream.peek() == std::ifstream::traits_type::eof())
        throw std::runtime_error(ERR_EMPTY);

    // Reading the entire file into a string, then removing all whitespace
    std::string file((std::istreambuf_iterator<char>(inputFileStream)), std::istreambuf_iterator<char>());
    file.erase(remove_if(file.begin(), file.end(), isspace), file.end());

    inputFileStream.close();

    _content = file;
    _itr = _content.begin();
}

/**
 * @brief Parse the JSON string and also check if there are any characters left after parsing
 * 
 * @return JsonValue The main Json object
*/
JsonValue JsonParser::parse(void) {
    std::cout << "Parsing JSON" << std::endl;

    JsonValue json = parseValue();

    if (_itr != _content.end())
        throw std::runtime_error(ERR_JSON_PARSE);

    if (json.getType() != JsonValue::JSON_OBJECT)
        throw std::runtime_error(ERR_JSON_TYPE);

    if (json.asObject().empty())
        throw std::runtime_error(ERR_EMPTY);

    std::cout << G << "JSON parsed successfully" << RE << std::endl;

    return json;
}

/**
 * @brief Parse a JSON value from the JSON string. Lots of recursion occurs here.
 * 
 * @return JsonValue The parsed JSON value
*/
JsonValue JsonParser::parseValue(void) {
    if (*_itr == '\"')
        return parseString();
    else if (isdigit(*_itr))
        return parseNumber();
    else if (*_itr == 'f' || *_itr == 't')
        return parseBoolean();
    else if (std::string(_itr, _itr + 4) == "null")
        return parseNull();
    else if (*_itr == '[')
        return parseArray();
    else if (*_itr == '{')
        return parseObject();
    else
        throw std::runtime_error(ERR_JSON_PARSE);
}

/**
 * @brief Parse a null from the JSON string
 * stringIterator _itr for nulls should start at 'n' and end at 'l'
 * then the next character should be ',' or '}' or EOF
 * 
 * @return JsonValue The parsed null value
*/
JsonValue JsonParser::parseNull(void) {
    _itr += 4; // move from 'n' to after 'l'
    if (!isEndOrDelimiter())
        throw std::runtime_error(ERR_JSON_PARSE);

    return JsonValue();
}

/**
 * @brief Parse a string from the JSON string
 * stringIterator _itr for strings should start at '\"' and end at '\"'
 * then the next character should be ',' or '}' or ':' or ']' or EOF
 * 
 * @return JsonValue The parsed string
*/
JsonValue JsonParser::parseString(void) {
    _itr++; // move from '\"'

    std::string::iterator tempItr = _itr;

    while (_itr != _content.end() && *_itr != '\"')
        _itr++;
    if (*_itr != '\"')
        throw std::runtime_error(ERR_JSON_QUOTE);

    std::string str(tempItr, _itr);

    _itr++; // move from '\"'
    if (*_itr != ':' && !isEndOrDelimiter())
        throw std::runtime_error(ERR_JSON_PARSE);

    return JsonValue(str);
}

/**
 * @brief Parse a number from the JSON string
 * stringIterator _itr for numbers should start at a digit and end at a non-digit
 * then the next character should be ',' or '}' or ']' or EOF
 * 
 * @return JsonValue The parsed number
*/
JsonValue JsonParser::parseNumber(void) {
    std::string::iterator tempItr = _itr;

    while (_itr != _content.end() && isdigit(*_itr))
        _itr++;

    int number = std::atoi(std::string(tempItr, _itr).c_str());

    if (!isEndOrDelimiter())
        throw std::runtime_error(ERR_JSON_PARSE);

    return JsonValue(number);
}

/**
 * @brief Parse a boolean from the JSON string
 * stringIterator _itr for booleans should start at 't' or 'f' and end at 'e'
 * then the next character should be ',' or '}' or ']' or EOF
 * 
 * @return JsonValue The parsed boolean
*/
JsonValue JsonParser::parseBoolean(void) {
    bool boolean;
    if (std::string(_itr, _itr + 4) == "true")
        boolean = true;
    else if (std::string(_itr, _itr + 5) == "false")
        boolean = false;
    else
        throw std::runtime_error(ERR_JSON_PARSE);

    _itr += (boolean ? 4 : 5);
    if (!isEndOrDelimiter())
        throw std::runtime_error(ERR_JSON_PARSE);

    return JsonValue(boolean);
}

/**
 * @brief Parse an array from the JSON string
 * stringIterator _itr for arrays should start at '[' and end at ']'
 * then the next character should be ',' or '}' or '[' or EOF
 * 
 * @return JsonValue The parsed array
*/
JsonValue JsonParser::parseArray(void) {
    _itr++; // move from '['

    std::vector<JsonValue> array;

    while (_itr != _content.end() && *_itr != ']') {
        array.push_back(parseValue());

        if (*_itr == ',') {
            _itr++; // move from ','
            continue;
        }

        if (*_itr != ']')
            throw std::runtime_error(ERR_JSON_PARSE);
    }

    _itr++; // move from ']'
    if (!isEndOrDelimiter())
        throw std::runtime_error(ERR_JSON_PARSE);

    return JsonValue(array);
}

/**
 * @brief Parse an object from the JSON string
 * stringIterator _itr for objects should start at '{' and end at '}'
 * then the next character should be ',' or '}' or '[' or EOF
 * 
 * @return JsonValue The parsed object
*/
JsonValue JsonParser::parseObject(void) {
    _itr++; // move from '{'

    std::map<std::string, JsonValue> objectMap;

    while (_itr != _content.end() && *_itr != '}') {
        if (*_itr != '\"')
            throw std::runtime_error(ERR_JSON_PARSE);

        std::string key = parseString().asString();
        _itr++; // move from ':'

        objectMap[key] = parseValue();

        if (*_itr == ',') {
            _itr++; // move from ','
            continue;
        }

        if (*_itr != '}')
            throw std::runtime_error(ERR_JSON_PARSE);
    }

    _itr++; // move from '}'
    if (!isEndOrDelimiter())
        throw std::runtime_error(ERR_JSON_PARSE);

    return JsonValue(objectMap);
}
