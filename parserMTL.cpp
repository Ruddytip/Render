#include <fstream>
#include <sstream>
#include "parserMTL.hpp"

ParserMTL::ParserMTL(std::vector<material>* _data) : data(_data){

}

ParserMTL::~ParserMTL(){

}

void ParserMTL::parserFile(const std::string& directory, const std::string& filename){
    std::ifstream in;
    in.open(directory + filename, std::ifstream::in);
    if(in.fail()) return;
    bool mat_flag = false;
            material m = {
            "Default",
            Vec3d(0.0, 0.0, 0.0),
            Vec3d(0.0, 0.0, 0.0),
            Vec3d(0.0, 0.0, 0.0),
            0.0,
            1.0,
            1.0,
            1,
            "Default",
            "Default",
            "Default",
            "Default",
            "Default",
        };
    while(!in.eof()){
        std::string line, trash;
        std::getline(in, line);
        std::stringstream iss(line);

        if(!line.compare(0, 7, "newmtl ")){    
            iss >> trash; iss >> trash;
            m.name = trash;
            mat_flag = true;
        } else
        if(!line.compare(0, 3, "Ka ")){
            iss >> trash;
            iss >> trash; m.Ka.x = std::stod(trash);
            iss >> trash; m.Ka.y = std::stod(trash);
            iss >> trash; m.Ka.z = std::stod(trash);
        } else
        if(!line.compare(0, 3, "Kd ")){
            iss >> trash;
            iss >> trash; m.Kd.x = std::stod(trash);
            iss >> trash; m.Kd.y = std::stod(trash);
            iss >> trash; m.Kd.z = std::stod(trash);
        } else
        if(!line.compare(0, 2, "Ks ")){
            iss >> trash;
            iss >> trash; m.Ks.x = std::stod(trash);
            iss >> trash; m.Ks.y = std::stod(trash);
            iss >> trash; m.Ks.z = std::stod(trash);
        } else
        if(!line.compare(0, 2, "Ns ")){
            iss >> trash; iss >> trash;
            m.Ns = std::stod(trash);
        } else
        if(!line.compare(0, 2, "Ni ")){
            iss >> trash; iss >> trash;
            m.Ni = std::stod(trash);
        } else
        if(!line.compare(0, 2, "d ")){
            iss >> trash; iss >> trash;
            m.d = std::stod(trash);
        } else
        if(!line.compare(0, 2, "Tr ")){
            iss >> trash; iss >> trash;
            m.d = std::stod(trash);
        } else
        if(!line.compare(0, 6, "illum ")){
            iss >> trash; iss >> trash;
            m.illum = std::stoi(trash);
        } else
        if(!line.compare(0, 7, "map_Ka ")){
            iss >> trash; iss >> trash;
            m.map_Ka = directory + "/textures/" + trash;
        } else
        if(!line.compare(0, 7, "map_Kd ")){
            iss >> trash; iss >> trash;
            m.map_Kd = directory + "/textures/" + trash;
        } else
        if(!line.compare(0, 7, "map_Ks ")){
            iss >> trash; iss >> trash;
            m.map_Ks = directory + "/textures/" + trash;
        } else
        if(!line.compare(0, 6, "map_d ")){
            iss >> trash; iss >> trash;
            m.map_D = directory + "/textures/" + trash;
        } else
        if(!line.compare(0, 9, "map_Bump ")){
            iss >> trash; iss >> trash;
            m.map_Bump = directory + "/textures/" + trash;
        }
        if(mat_flag && (line.length() == 0)){
            data->push_back(m);
            mat_flag = false;
        }
    }
}