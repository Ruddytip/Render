#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include "model.hpp"

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

void TModel::triangle(BMP& image, const Vec3d* t, int id_f, BMP& texture){
    Vec2i scr(image.getWidth(), image.getHeight());
    Vec2i scrT(texture.getWidth(), texture.getHeight());

    Vec2d tmin(uv[faces[id_f].id_vt.x].x * scrT.x, uv[faces[id_f].id_vt.x].y * scrT.y);
    Vec2d tmax = tmin;
    tmin.x = (uv[faces[id_f].id_vt.y].x * scrT.x < tmin.x ? uv[faces[id_f].id_vt.y].x * scrT.x:  tmin.x);
    tmin.y = (uv[faces[id_f].id_vt.y].y * scrT.y < tmin.y ? uv[faces[id_f].id_vt.y].y * scrT.y:  tmin.y);

    tmin.x = (uv[faces[id_f].id_vt.z].x * scrT.x < tmin.x ? uv[faces[id_f].id_vt.z].x * scrT.x:  tmin.x);
    tmin.y = (uv[faces[id_f].id_vt.z].y * scrT.y < tmin.y ? uv[faces[id_f].id_vt.z].y * scrT.y:  tmin.y);

    tmax.x = (uv[faces[id_f].id_vt.y].x * scrT.x > tmax.x ? uv[faces[id_f].id_vt.y].x * scrT.x:  tmax.x);
    tmax.y = (uv[faces[id_f].id_vt.y].y * scrT.y > tmax.y ? uv[faces[id_f].id_vt.y].y * scrT.y:  tmax.y);

    tmax.x = (uv[faces[id_f].id_vt.z].x * scrT.x > tmax.x ? uv[faces[id_f].id_vt.z].x * scrT.x:  tmax.x);
    tmax.y = (uv[faces[id_f].id_vt.z].y * scrT.y > tmax.y ? uv[faces[id_f].id_vt.z].y * scrT.y:  tmax.y);

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
                    int x = range(i, Min.x, Max.x, tmin.x, tmax.x);
                    int y = range(j, Min.y, Max.y, tmin.y, tmax.y);
                    TColor color = texture.getPixel(x, y);
                    image.setPixel(i, j, color);
                }
        }
    }

}

void minMax(const Vec3d p, Vec3d& min, Vec3d& max){
    min.x = (p.x < min.x ? p.x : min.x); max.x = (p.x > max.x ? p.x : max.x);
    min.y = (p.y < min.y ? p.y : min.y); max.y = (p.y > max.y ? p.y : max.y);
    min.z = (p.z < min.z ? p.z : min.z); max.z = (p.z > max.z ? p.z : max.z);
}

void TModel::parser_VERTS(const std::string& line){
    std::stringstream iss(line);
    std::string trash;
    iss >> trash; Vec3d point;
    iss >> trash; point.x = std::stod(trash);
    iss >> trash; point.y = std::stod(trash);
    iss >> trash; point.z = std::stod(trash);
    minMax(point, min, max);
    verts.push_back(point);
}

void TModel::parser_VT(const std::string& line){
    std::stringstream iss(line);
    std::string trash;
    iss >> trash; Vec2d point;
    iss >> trash; point.x = std::stod(trash);
    iss >> trash; point.y = std::stod(trash);
    uv.push_back(point);
}

void TModel::parser_VN(const std::string& line){
    std::stringstream iss(line);
    std::string trash;
    iss >> trash; Vec3d point;
    iss >> trash; point.x = std::stod(trash);
    iss >> trash; point.y = std::stod(trash);
    iss >> trash; point.z = std::stod(trash);
    normals.push_back(point);
}

void TModel::parser_FACES(const std::string& line){
    std::stringstream iss(line);
    std::string trash;
    face fc;
    Vec3i point;
    Vec3i pointUV;
    Vec3i pointNRM;
    iss >> trash;

    iss >> trash;
    point.x    = std::stoi(trash.substr(0, trash.find('/'))) - 1;
    pointUV.x  = std::stoi(trash.substr(trash.find('/') + 1, trash.find_last_of('/'))) - 1;
    pointNRM.x = std::stoi(trash.substr(trash.find_last_of('/') + 1, trash.length() - 1)) - 1;


    iss >> trash;
    point.y    = std::stoi(trash.substr(0, trash.find('/'))) - 1;
    pointUV.y  = std::stoi(trash.substr(trash.find('/') + 1, trash.find_last_of('/'))) - 1;
    pointNRM.y = std::stoi(trash.substr(trash.find_last_of('/') + 1, trash.length() - 1)) - 1;

    iss >> trash;
    point.z    = std::stoi(trash.substr(0, trash.find('/'))) - 1;
    pointUV.z  = std::stoi(trash.substr(trash.find('/') + 1, trash.find_last_of('/'))) - 1;
    pointNRM.z = std::stoi(trash.substr(trash.find_last_of('/') + 1, trash.length() - 1)) - 1;

    fc.id_v  = point;
    fc.id_vt = pointUV;
    fc.id_vn = pointNRM;
    faces.push_back(fc);
}

void TModel::parser_OBJ(const std::string& filename){
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if(in.fail()) return;

    while(!in.eof()){
        std::string line;
        std::getline(in, line);
        if(!line.compare(0, 2, "v ")){
            parser_VERTS(line);
        }
        if(!line.compare(0, 3, "vt ")){
            parser_VT(line);
        }
        if(!line.compare(0, 3, "vn ")){
            parser_VN(line);
        }
        if(!line.compare(0, 2, "f ")){
            parser_FACES(line);
        }
    }

}

TModel::TModel(const std::string& filename){
    max.x = max.y = max.z = -std::numeric_limits<double>::max();
    min.x = min.y = min.z =  std::numeric_limits<double>::max();
    parser_OBJ(filename);
    size = max - min;
}

// void TModel::initMaterials(std::string filename){}

// void TModel::initGroups(){
//     if(nameModel == "airpods"){
//         // objects[0].visible = false; // Ãëàçà
//         // objects[1].visible = false; // Òåëî
//         // objects[2].visible = false; // Âîëîñû, áðîâè, ðåñíèöû
//         // objects[3].visible = false; // Èãðóøêè
//     }
//     if(nameModel == "anorexic"){
//         // objects[0].visible = false; // Òåëî
//         // objects[1].visible = false; // Ðåñíèöû?
//         // objects[2].visible = false; // Ãëàçà
//     }
//     if(nameModel == "elizabeth"){
//         // objects[0].visible = false;  //Òåëî
//         // objects[1].visible = false;  //Íàïàëü÷íèê
//         // objects[2].visible = false;  //Ðåñíèöû
//         // objects[3].visible = false;  //Âîëîñû
//         // objects[4].visible = false;  //Áðîø
//         // objects[5].visible = false;  //Ñîðî÷êà
//         // objects[6].visible = false;  //Þáêà
//         objects[7].visible = false;  //Òåëî
//         objects[8].visible = false;  //Òåëî
//         objects[9].visible = false;  //Ïîäòÿæêè
//         // objects[10].visible = false; //×óëêè
//         // objects[11].visible = false; //Òóôëè
//         objects[12].visible = false; //Öèëèíäð?
//     }
//     if(nameModel == "orc"){
//         // objects[0].visible = false;  // Ëåâûé ãëàç
//         // objects[1].visible = false;  // Ïðàâûé ãëàç
//         // objects[2].visible = false;  // Òåëî
//         // objects[3].visible = false;  // Äåñíà
//         // objects[4].visible = false;  // Âåðõíèé çóá
//         // objects[5].visible = false;  // Âåðõíèé çóá
//         // objects[6].visible = false;  // Âåðõíèé çóá
//         // objects[7].visible = false;  // Âåðõíèé çóá
//         // objects[8].visible = false;  // Âåðõíèé çóá
//         // objects[9].visible = false;  // Âåðõíèé çóá
//         // objects[10].visible = false;  // Íèæíèé çóá
//         // objects[11].visible = false;  // Íèæíèé çóá
//         // objects[12].visible = false;  // Íèæíèé çóá
//         // objects[13].visible = false;  // Íèæíèé çóá
//         // objects[14].visible = false;  // Íèæíèé çóá
//         // objects[15].visible = false;  // Íèæíèé çóá
//         // objects[16].visible = false;  // Êëûê ïðàâûé
//         // objects[17].visible = false;  // Êëûê ëåâûé
//         // objects[18].visible = false;  // Êîëüöî â ïðàâîì óõå çàäíåå
//         // objects[19].visible = false;  // Êîëüöî â ïðàâîì óõå ïåðåäíåå
//         // objects[20].visible = false;  // Êîëüöî â ëåâîì óõå çàäíåå
//         // objects[21].visible = false;  // Êîëüöî â ëåâîì óõå ïåðåäíåå
//         // objects[22].visible = false;  // Êîëüöî â êëûêå
//         // objects[23].visible = false;  // Êîëüöî â íîñó
//     }
// }

// void TModel::printInfo(){
//     std::cout << nameModel << std::endl;
//     std::cout << "Objects count - " << objects.size() << std::endl;
//     for(unsigned int i = 0; i < objects.size(); ++i)
//             std::cerr << i << " - " << objects[i].name << ": faces - " << objects[i].faces.size() << std::endl;
//     std::cout << "Verts count - " << verts.size() << std::endl;
//     // std::cout << "Materal count - " << materials.size() << std::endl;
//     // for(unsigned int i = 0; i < textures.size(); ++i) std::cout << i << " - " << textures[i].name << std::endl;
// }

TModel::~TModel() {
    delete[] z_buffer;
}

void TModel::drawZ_buffer(BMP& image){
    int width = image.getWidth(), height = image.getHeight();
    double mn, mx;
    mn = mx = z_buffer[0];
    for(int j = 0; j < height; ++j)
        for(int i = 0; i < width; ++i){
            TColor color{120, 0, 120};
            if(z_buffer[i + j * width] != -std::numeric_limits<double>::max()){
                byte1 c = range(z_buffer[i + j * width] + min.z, min.z, max.z, 0, 255);
                color = {c, c, c}; 
            }
            image.setPixel(i, j, color);
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

// void TModel::drawMaterial(HDC hdc){
// for(int i = 0; i < size_screen.x * size_screen.y; ++i) z_buffer[i] = -std::numeric_limits<double>::max();
//     for(unsigned int id_o = 0; id_o < objects.size(); ++id_o){
//         if(!objects[id_o].visible) continue;
//         for (unsigned int i = 0; i < objects[id_o].faces.size(); ++i) {
//             Vec3d screen_coords[3];
//             Vec3d world_coords[3];
//             for (int j = 0; j < 3; j++) {
//                 world_coords[j] = verts[objects[id_o].faces[i].id_v[j]];
//                 screen_coords[j] = (world_coords[j] - min) * scale;
//                 screen_coords[j].y = size_screen.y - screen_coords[j].y;
//             }
//             Vec3d normal = (world_coords[0] - world_coords[1]) ^ (world_coords[0] - world_coords[2]);
//             double intensity = normal.getCosAngle(Vec3d(0.0, 0.0, 1.0));
//             int c = 255 / 2;
//             if(materials.size() > 1)
//                 c = range(objects[id_o].faces[i].id_m, 0, materials.size()-1, 0, 255);
//             if(intensity > 0)
//                 triangle(hdc, screen_coords, RGB(c, c, c));
//         }
//     }
// }

void TModel::drawMesh(BMP& image){
    Vec2i scr(image.getWidth() / 2, image.getHeight() / 2);
    Vec3d pos = Vec3d(0.0, 0.0, 0.0) - min;
    
    for (unsigned long id_f = 0; id_f < faces.size(); ++id_f) {
        Vec2i screen_coords[3];
        for (int id_v = 0; id_v < 3; ++id_v) {
            Vec3d world_coords = verts[faces[id_f].id_v[id_v]] + pos;
            screen_coords[id_v] = Vec2i(world_coords.x * scr.x, world_coords.y * scr.y);

        }
        line(image, screen_coords[0], screen_coords[1], TColor{255, 255, 255});
        line(image, screen_coords[1], screen_coords[2], TColor{255, 255, 255});
        line(image, screen_coords[2], screen_coords[0], TColor{255, 255, 255});
    }
}

void TModel::drawMeshTriangle(BMP& image){
    Vec2i scr(image.getWidth(), image.getHeight());
    Vec3d pos = Vec3d(0.0, 0.0, 0.0) - min;
    z_buffer = new double[scr.x * scr.y];
    for(int i = 0; i < scr.x * scr.y; ++i) z_buffer[i] = -std::numeric_limits<double>::max();

        for(unsigned int id_f = 0; id_f < faces.size(); ++id_f){
            Vec3d screen_coords[3];
            Vec3d world_coords[3];
            for (int id_v = 0; id_v < 3; ++id_v) {
                world_coords[id_v] = verts[faces[id_f].id_v[id_v]] + pos;
                screen_coords[id_v] = Vec3d(world_coords[id_v].x * scr.x / 2, world_coords[id_v].y * scr.y / 2, world_coords[id_v].z);
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
    Vec3d pos = Vec3d(0.0, 0.0, 0.0) - min;
    z_buffer = new double[scr.x * scr.y];
    for(int i = 0; i < scr.x * scr.y; ++i) z_buffer[i] = -std::numeric_limits<double>::max();

    for (unsigned int id_f = 0; id_f < faces.size(); ++id_f){
        Vec3d screen_coords[3];
        Vec3d world_coords[3];
        for (int id_v = 0; id_v < 3; id_v++) {
            world_coords[id_v] = verts[faces[id_f].id_v[id_v]] + pos;
            screen_coords[id_v] = Vec3d(world_coords[id_v].x * scr.x / 2, world_coords[id_v].y * scr.y / 2, world_coords[id_v].z);
        }
        Vec3d normal = (world_coords[0] - world_coords[1]) ^ (world_coords[0] - world_coords[2]);
        double intensity = normal.getCosAngle(Vec3d(0.0, 0.0, 1.0));
        if(intensity > 0)
            triangle(image, screen_coords, id_f, texture);
    }

}