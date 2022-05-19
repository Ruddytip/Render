#pragma once
#include <string>
#include "geometry.hpp"

struct MetaInfoOBJ{
    std::vector<Vec3d> verts;
	std::vector<Vec2d> uv;
	std::vector<Vec3d> normals;
	std::vector<face> faces;
    std::vector<material> materials;
    Vec3d min, max;
	Vec3d size;
};

class ParserOBJ{
private:
    MetaInfoOBJ* data;
    void minMax(const Vec3d p, Vec3d& min, Vec3d& max);
public:
    ParserOBJ(MetaInfoOBJ* _data);
    ~ParserOBJ();
    void parserFile(const std::string& filename);
    void parser_VERTS(const std::string& line);
	void parser_VT(const std::string& line);
	void parser_VN(const std::string& line);
	void parser_FACES(const std::string& line);
    void parser_MTL(const std::string& line);
};