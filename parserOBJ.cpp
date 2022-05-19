#include <fstream>
#include <sstream>
#include "parserOBJ.hpp"

void ParserOBJ::minMax(const Vec3d p, Vec3d& min, Vec3d& max){
    min.x = (p.x < min.x ? p.x : min.x); max.x = (p.x > max.x ? p.x : max.x);
    min.y = (p.y < min.y ? p.y : min.y); max.y = (p.y > max.y ? p.y : max.y);
    min.z = (p.z < min.z ? p.z : min.z); max.z = (p.z > max.z ? p.z : max.z);
}

ParserOBJ::ParserOBJ(MetaInfoOBJ* _data) : data(_data){

}

ParserOBJ::~ParserOBJ(){

}

void ParserOBJ::parserFile(const std::string& filename){
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if(in.fail()) return;

    while(!in.eof()){
        std::string line;
        std::getline(in, line);
        if(!line.compare(0, 2, "v ")){
            parser_VERTS(line);
        } else
        if(!line.compare(0, 3, "vt ")){
            parser_VT(line);
        } else
        if(!line.compare(0, 3, "vn ")){
            parser_VN(line);
        } else
        if(!line.compare(0, 2, "f ")){
            parser_FACES(line);
        }
    }
}

void ParserOBJ::parser_VERTS(const std::string& line){
    std::stringstream iss(line);
    std::string trash;
    iss >> trash; Vec3d point;
    iss >> trash; point.x = std::stod(trash);
    iss >> trash; point.y = std::stod(trash);
    iss >> trash; point.z = std::stod(trash);
    minMax(point, data->min, data->max);
    data->verts.push_back(point);
}

void ParserOBJ::parser_VT(const std::string& line){
    std::stringstream iss(line);
    std::string trash;
    iss >> trash; Vec2d point;
    iss >> trash; point.x = std::stod(trash);
    iss >> trash; point.y = std::stod(trash);
    if(point.y > 1) point.y = point.y - 1.;
    data->uv.push_back(point);
}

void ParserOBJ::parser_VN(const std::string& line){
    std::stringstream iss(line);
    std::string trash;
    iss >> trash; Vec3d point;
    iss >> trash; point.x = std::stod(trash);
    iss >> trash; point.y = std::stod(trash);
    iss >> trash; point.z = std::stod(trash);
    data->normals.push_back(point);
}

void ParserOBJ::parser_FACES(const std::string& line){
    std::stringstream iss(line);
    std::string trash;
    face fc; iss >> trash;
    unsigned int V[4];
    unsigned int UV[4];
    unsigned int NRM[4];

    for(int i = 0; i < 4; ++i){
        iss >> trash;
        if(trash.empty()) break;
        V[i]   = std::stoi(trash.substr(0, trash.find('/'))) - 1;
        std::string dataUV = trash.substr(trash.find('/') + 1, trash.find_last_of('/'));
        UV[i]  = (dataUV.empty() ? 0 : std::stoi(dataUV) - 1);
        NRM[i] = std::stoi(trash.substr(trash.find_last_of('/') + 1, trash.length() - 1)) - 1;
    }

    fc.id_v  = Vec3i{V[0],   V[1],   V[2]};
    fc.id_vt = Vec3i{UV[0],  UV[1],  UV[2]};
    fc.id_vn = Vec3i{NRM[0], NRM[1], NRM[2]};
    data->faces.push_back(fc);

    if(!trash.empty()){
        fc.id_v  = Vec3i{V[0],   V[2],   V[3]};
        fc.id_vt = Vec3i{UV[0],  UV[2],  UV[3]};
        fc.id_vn = Vec3i{NRM[0], NRM[2], NRM[3]};
        data->faces.push_back(fc);
    }
}
