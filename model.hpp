#pragma once
#include "BMP.hpp"
#include "geometry.hpp"

class TModel{
    private:
	// ==========================================================
	std::vector<Vec3d> verts;
	std::vector<Vec2d> uv;
	std::vector<Vec3d> normals;
	std::vector<face> faces;
	std::vector<material> materials;
	// std::vector<object> objects;
	// std::vector<texture> textures;
	Vec3d min, max;
	Vec3d size;
	double* z_buffer;
	// ==========================================================
	void line(BMP& image, Vec2i t0, Vec2i t1, const TColor& color);
	void triangle(BMP& image, const Vec3d* t, const TColor& color);
	void drawFace(BMP& image, const Vec3d* t, const int id_f, BMP& texture, const double intensity);
	bool helpZ(Vec2i scr, const Vec2i point, const Vec3d* t);
	TColor getColorTexture(const Vec2i point, const Vec3d* t, const int id_f, BMP& texture);
	// ==========================================================
	void parserOBJFile(const std::string& filename);
	void parserMTLFile(const std::string& filename);
	// ==========================================================
public:
	TModel(const std::string& filename);
	~TModel();
	void drawMesh(BMP& image);
	void drawMeshTriangle(BMP& image);
	void drawMeshTexture(BMP& image, BMP& texture);
	void drawZ_buffer(BMP& image);
	void draw_UV_map(BMP& image);
	void prnmaterial();
};