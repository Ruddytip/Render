#pragma once
#include "BMP.hpp"
#include "geometry.hpp"

class TModel{
    private:
	// std::string nameModel;
	std::vector<Vec3d> verts;
	std::vector<Vec3i> faces;
	// std::vector<Vec2d> uv;
	// std::vector<Vec3d> normals;
	// std::vector<object> objects;
	// std::vector<texture> textures;
	// std::vector<material> materials;
	Vec3d min, max;
	Vec3d size;
	// double scale;
	double* z_buffer;
	void line(BMP& image, Vec2i t0, Vec2i t1, const TColor& color);
	void triangle(BMP& image, const Vec3d* t, const TColor& color);
	// void triangle(BMP& image, const Vec3d* t, int count, int n, double nor);
	bool helpZ(BMP& image, const Vec2i point, const Vec3d* t);
	// void printInfo();
	// void initGroups();
	// void initMaterials(std::string filename);
	void parser_OBJ(const std::string& filename);
public:
	TModel(const std::string& filename);
	~TModel();
	void drawMesh(BMP& image);
	void drawMeshTriangle(BMP& image);
	// void drawMeshTexture(BMP& image);
	void drawZ_buffer(BMP& image);
	// void drawMaterial();
};