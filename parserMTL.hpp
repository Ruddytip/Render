#pragma once
#include <string>
#include "geometry.hpp"

class ParserMTL{
private:
    material* data;
public:
    ParserMTL(material* _data);
    ~ParserMTL();
    void parserFile(const std::string& filename);
};