#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "WareHouse.h"

class Parser {
public:
    static bool parseConfigFile(const std::string& filePath, WareHouse& wareHouse);
};

#endif
