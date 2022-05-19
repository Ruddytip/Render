#pragma once
#include <string>
#include "geometry.hpp"

class ParserMTL{
private:
    std::vector<material>* data;
public:
    ParserMTL(std::vector<material>* _data);
    ~ParserMTL();
    void parserFile(const std::string& directory, const std::string& filename);
};