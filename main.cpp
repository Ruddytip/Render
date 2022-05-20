#include "model.hpp"

void line(BMP& image, Vec2i t0, Vec2i t1, const TColor& color) {
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

int main(){
    const unsigned long width  = 1000;
    const unsigned long height = 1000;
    BMP output(width, height, TColor{100, 200, 100});

    TModel model("obj/african_head");
    // TModel model("obj/tennyson");
    // TModel model("test.obj");

    BMP texture("obj/african_head/textures/african_head_diffuse.bmp");
    // BMP texture("obj/african_head/textures/african_head_nm_tangent.bmp");
    // BMP texture("obj/african_head/textures/african_head_spec.bmp");
    // BMP texture("obj/tennyson/textures/hair.bmp");
    // BMP texture("obj/tennyson/textures/body.bmp");
    // BMP texture("obj/tennyson/textures/shirt.bmp");
    // BMP texture("obj/tennyson/textures/pants.bmp");

    // BMP texture("test_1.bmp");
    // BMP texture("test_2.bmp");

    // model.draw_UV_map(output);
    // model.drawMesh(output);
    // model.drawMeshTriangle(output);
    model.drawMeshTexture(output, texture);

    output.writeToFile("output.bmp");

    // BMP zbf(width, height, TColor{120, 0, 120});
    // model.drawZ_buffer(zbf);
    // zbf.writeToFile("zbf.bmp");

    return 0;
}