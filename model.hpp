#pragma once
#include "BMP.hpp"
#include "geometry.hpp"

class TModel{
    private:
	std::vector<Vec3d> verts;
	std::vector<Vec2d> uv;
	std::vector<Vec3d> normals;
	std::vector<face> faces;
	// std::vector<object> objects;
	// std::vector<texture> textures;
	// std::vector<material> materials;
	Vec3d min, max;
	Vec3d size;
	double* z_buffer;
	void line(BMP& image, Vec2i t0, Vec2i t1, const TColor& color);
	void triangle(BMP& image, const Vec3d* t, const TColor& color);
	void triangle(BMP& image, const Vec3d* t, int id_f, BMP& texture);
	bool helpZ(Vec2i scr, const Vec2i point, const Vec3d* t);
	// void printInfo();
	// void initGroups();
	// void initMaterials(std::string filename);
	void parser_OBJ(const std::string& filename);
	void parser_VERTS(const std::string& line);
	void parser_VT(const std::string& line);
	void parser_VN(const std::string& line);
	void parser_FACES(const std::string& line);
public:
	TModel(const std::string& filename);
	~TModel();
	void drawMesh(BMP& image);
	void drawMeshTriangle(BMP& image);
	void drawMeshTexture(BMP& image, BMP& texture);
	void drawZ_buffer(BMP& image);
	void draw_UV_map(BMP& image);
	// void drawMaterial();
};