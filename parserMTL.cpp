#include <fstream>
#include <sstream>
#include "parserMTL.hpp"

void ParserMTL::parserFile(const std::string& filename){
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if(in.fail()) return;

    while(!in.eof()){
        std::string line;
        std::getline(in, line);
        if(!line.compare(0, 7, "newmtl ")){
            
        } else
        if(!line.compare(0, 3, "Ka ")){
            
        } else
        if(!line.compare(0, 3, "Kd ")){
            
        } else
        if(!line.compare(0, 2, "Ks ")){
            
        } else
        if(!line.compare(0, 2, "Ns ")){
            
        } else
        if(!line.compare(0, 2, "Ni ")){
            
        } else
        if(!line.compare(0, 2, "d ")){
            
        } else
        if(!line.compare(0, 2, "Tr ")){
            
        } else
        if(!line.compare(0, 6, "illum ")){
            
        } else
        if(!line.compare(0, 7, "map_Ka ")){
            
        } else
        if(!line.compare(0, 7, "map_Kd ")){
            
        } else
        if(!line.compare(0, 7, "map_Ks ")){
            
        } else
        if(!line.compare(0, 6, "map_D ")){
            
        } else
        if(!line.compare(0, 9, "map_Bump ")){
            
        }
    }
}