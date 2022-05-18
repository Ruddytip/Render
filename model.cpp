#include <iostream>
#include <limits>
#include "model.hpp"
#include "parserOBJ.hpp"

void TModel::line(BMP& image, Vec2i t0, Vec2i t1, const TColor& color) {
    int x0 = t0.x, y0 = t0.y;
    int x1 = t1.x, y1 = t1.y;
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        image.setPixel((steep?y:x), (steep?x:y), color);
        error2 += derror2;

        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

double range(double data, double old_min, double old_max, double new_min, double new_max){
    double old_range = old_max - old_min;
    double new_range = new_max - new_min;
    return (((data - old_min) * new_range) / old_range) + new_min;
}

double helpPointPos(const Vec2i p, const Vec3d a, const Vec3d b){
    return (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
}

bool inTriangle(const Vec2i p, const Vec3d* t){
    double q0 = helpPointPos(p, t[0], t[1]);
    double q1 = helpPointPos(p, t[1], t[2]);
    double q2 = helpPointPos(p, t[2], t[0]);
    return (q0 >= 0 && q1 >= 0 && q2 >= 0) || (q0 <= 0 && q1 <= 0 && q2 <= 0);
}

bool TModel::helpZ(const Vec2i scr, const Vec2i point, const Vec3d* t){
    if(point.x < 0 || point.y < 0 || point.x >= scr.x || point.y >= scr.y) return false;
    Vec3d normal = (t[0] - t[1]) ^ (t[0] - t[2]);
    double z = (-normal.x * (point.x - t[0].x) - normal.y * (point.y - t[0].y) + normal.z * t[0].z) / normal.z;
    unsigned long index = point.x + point.y * scr.x;
        if(z > z_buffer[index]){
            z_buffer[index] = z;
            return true;
        }
    return false;
}

void TModel::triangle(BMP& image, const Vec3d* t, const TColor& color){
    Vec2i scr(image.getWidth(), image.getHeight());
    Vec3d Min = t[0], Max = t[0];
    for(int i = 1; i < 3; ++i){
        Min.x = t[i].x < Min.x? t[i].x: Min.x; Max.x = t[i].x > Max.x? t[i].x: Max.x;
        Min.y = t[i].y < Min.y? t[i].y: Min.y; Max.y = t[i].y > Max.y? t[i].y: Max.y;
    }
    for(int j = Min.y; j <= Max.y; ++j){
        for(int i = Min.x; i <= Max.x; ++i){
            Vec2i point(i, j);
            if(inTriangle(point, t))
                if(helpZ(scr, point, t))
                    image.setPixel(i, j, color);
        }
    }
}

TColor TModel::getColorTexture(const Vec2i point, const Vec3d* t, const int id_f, BMP& texture){
    Vec2d p(point.x - t[0].x, point.y - t[0].y);
    Vec3d AB(t[1] - t[0]);
    Vec3d AC(t[2] - t[0]);
    double w1 = (p.y * AC.x - p.x * AC.y) / (AB.y * AC.x - AB.x * AC.y);
    double w2 = (p.y - w1 * AB.y) / AC.y;

    Vec2d u[3];
    for(int i = 0; i < 3; ++i){
        u[i] = Vec2d(uv[faces[id_f].id_vt[i]].x, uv[faces[id_f].id_vt[i]].y);
    }

    Vec2d t_AB(u[1] - u[0]);
    Vec2d t_AC(u[2] - u[0]);
    Vec2d t_point(u[0].x + t_AB.x * w1 + t_AC.x * w2, u[0].y + t_AB.y * w1 + t_AC.y * w2);

    return texture.getPixel(t_point.x * texture.getWidth(), t_point.y * texture.getHeight());
}

void TModel::drawFace(BMP& image, const Vec3d* t, const int id_f, BMP& texture, const double intensity){
    Vec2i scr(image.getWidth(), image.getHeight());
    Vec3d Min = t[0], Max = t[0];
    for(int i = 1; i < 3; ++i){
        Min.x = t[i].x < Min.x? t[i].x: Min.x; Max.x = t[i].x > Max.x? t[i].x: Max.x;
        Min.y = t[i].y < Min.y? t[i].y: Min.y; Max.y = t[i].y > Max.y? t[i].y: Max.y;
    }
    for(int j = Min.y; j <= Max.y; ++j){
        for(int i = Min.x; i <= Max.x; ++i){
            Vec2i point(i, j);
            if(inTriangle(point, t))
                if(helpZ(scr, point, t)){                    
                    TColor color = getColorTexture(point, t, id_f, texture);
                    byte1 c1 = color.R * intensity;
                    byte1 c2 = color.G * intensity;
                    byte1 c3 = color.B * intensity;
                    color = TColor{c1, c2, c3};
                    image.setPixel(i, j, color);
                }
        }
    }
}

void TModel::parserOBJFile(const std::string& filename){
    MetaInfoOBJ _data;
    ParserOBJ prs(&_data);
    prs.parserFile(filename);
    verts = _data.verts;
    uv = _data.uv;
    normals = _data.normals;
    faces = _data.faces;
    min = _data.min;
    max = _data.max;
    size = _data.size;
}

TModel::TModel(const std::string& filename){
    max.x = max.y = max.z = -std::numeric_limits<double>::max();
    min.x = min.y = min.z =  std::numeric_limits<double>::max();
    parserOBJFile(filename);
    size = max - min;

    double f = 0;
    if(f != 0){
        f = f * 3.141592 / 180;
        for(int i = 0; i < verts.size(); ++i){
            verts[i].x = cos(f) * verts[i].x - sin(f) * verts[i].z;
            verts[i].z = sin(f) * verts[i].x + cos(f) * verts[i].z;
        }
    }

}

TModel::~TModel() {
    delete[] z_buffer;
}

void TModel::drawZ_buffer(BMP& image){
    int width = image.getWidth(), height = image.getHeight();
    double mn, mx;
    mn = mx = z_buffer[0];
    for(int j = 0; j < height; ++j)
        for(int i = 0; i < width; ++i){
            if(z_buffer[i + j * width] != -std::numeric_limits<double>::max()){
                byte1 c = range(z_buffer[i + j * width], min.z, max.z, 0, 255);
                image.setPixel(i, j, TColor{c, c, c});
            }
        }
}

void TModel::draw_UV_map(BMP& image){
    Vec2i scr(image.getWidth(), image.getHeight());

    for (unsigned int id_f = 0; id_f < faces.size(); ++id_f){
        line(image, Vec2i(uv[faces[id_f].id_vt.x].x * scr.x, uv[faces[id_f].id_vt.x].y * scr.y), Vec2i(uv[faces[id_f].id_vt.y].x * scr.x, uv[faces[id_f].id_vt.y].y * scr.y), TColor{255, 0, 0});
        line(image, Vec2i(uv[faces[id_f].id_vt.y].x * scr.x, uv[faces[id_f].id_vt.y].y * scr.y), Vec2i(uv[faces[id_f].id_vt.z].x * scr.x, uv[faces[id_f].id_vt.z].y * scr.y), TColor{255, 0, 0});
        line(image, Vec2i(uv[faces[id_f].id_vt.z].x * scr.x, uv[faces[id_f].id_vt.z].y * scr.y), Vec2i(uv[faces[id_f].id_vt.x].x * scr.x, uv[faces[id_f].id_vt.x].y * scr.y), TColor{255, 0, 0});
    }
}

void TModel::drawMesh(BMP& image){
    Vec2i scr(image.getWidth(), image.getHeight());
    Vec2d scale(
        (size.x / size.y) > 1 ? 1 : (size.x / size.y),
        (size.y / size.x) > 1 ? 1 : (size.y / size.x)
    );
    Vec2d position(
        (scr.x - (scr.x * scale.x)) / 2,
        (scr.y - (scr.y * scale.y)) / 2
    );

    for (unsigned long id_f = 0; id_f < faces.size(); ++id_f) {
        Vec2i screen_coords[3];
        for (int id_v = 0; id_v < 3; ++id_v) {
            double x = range(verts[faces[id_f].id_v[id_v]].x, min.x, max.x, 0, scr.x) * scale.x + position.x;
            double y = range(verts[faces[id_f].id_v[id_v]].y, min.y, max.y, 0, scr.y) * scale.y + position.y;
            screen_coords[id_v] = Vec2i(x, y);

        }
        line(image, screen_coords[0], screen_coords[1], TColor{255, 255, 255});
        line(image, screen_coords[1], screen_coords[2], TColor{255, 255, 255});
        line(image, screen_coords[2], screen_coords[0], TColor{255, 255, 255});
    }
}

void TModel::drawMeshTriangle(BMP& image){
    Vec2i scr(image.getWidth(), image.getHeight());
    Vec3d scale(
        (size.x / size.y) > 1 ? 1 : (size.x / size.y),
        (size.y / size.x) > 1 ? 1 : (size.y / size.x),
        1
    );
    Vec3d position(
        (scr.x - (scr.x * scale.x)) / 2,
        (scr.y - (scr.y * scale.y)) / 2,
        1
    );

    z_buffer = new double[scr.x * scr.y];
    for(int i = 0; i < scr.x * scr.y; ++i) z_buffer[i] = -std::numeric_limits<double>::max();

        for(unsigned int id_f = 0; id_f < faces.size(); ++id_f){
            Vec3d screen_coords[3];
            Vec3d world_coords[3];
            for (int id_v = 0; id_v < 3; ++id_v) {
                world_coords[id_v] = verts[faces[id_f].id_v[id_v]];
                double x = range(verts[faces[id_f].id_v[id_v]].x, min.x, max.x, 0, scr.x) * scale.x + position.x;
                double y = range(verts[faces[id_f].id_v[id_v]].y, min.y, max.y, 0, scr.y) * scale.y + position.y;
                screen_coords[id_v] = Vec3d(x, y, world_coords[id_v].z);
            }
            Vec3d normal = (world_coords[0] - world_coords[1]) ^ (world_coords[0] - world_coords[2]);
            double intensity = normal.getCosAngle(Vec3d(0.0, 0.0, 1.0));
            if (intensity > 0) {
                byte1 color = intensity * 255;
                triangle(image, screen_coords, TColor{color, color, color});
            }
        }
}

void TModel::drawMeshTexture(BMP& image, BMP& texture){
    Vec2i scr(image.getWidth(), image.getHeight());
    Vec3d scale(
        (size.x / size.y) > 1 ? 1 : (size.x / size.y),
        (size.y / size.x) > 1 ? 1 : (size.y / size.x),
        1
    );
    Vec3d position(
        (scr.x - (scr.x * scale.x)) / 2,
        (scr.y - (scr.y * scale.y)) / 2,
        1
    );

    z_buffer = new double[scr.x * scr.y];
    for(int i = 0; i < scr.x * scr.y; ++i) z_buffer[i] = -std::numeric_limits<double>::max();

    for (unsigned int id_f = 0; id_f < faces.size(); ++id_f){
        Vec3d screen_coords[3];
        Vec3d world_coords[3];
        for (int id_v = 0; id_v < 3; id_v++) {
            world_coords[id_v] = verts[faces[id_f].id_v[id_v]];
            double x = range(verts[faces[id_f].id_v[id_v]].x, min.x, max.x, 0, scr.x) * scale.x + position.x;
            double y = range(verts[faces[id_f].id_v[id_v]].y, min.y, max.y, 0, scr.y) * scale.y + position.y;
            screen_coords[id_v] = Vec3d(x, y, world_coords[id_v].z);
        }
        Vec3d normal = (world_coords[1] - world_coords[0]) ^ (world_coords[2] - world_coords[0]);
        double intensity = normal.getCosAngle(Vec3d(0.0, 0.0, 1.0));
        if(intensity > 0)
            drawFace(image, screen_coords, id_f, texture, intensity);
    }

}