#include "model.hpp"

int main(){
    const unsigned long width  = 1000;
    const unsigned long height = 1000;
    BMP output(width, height, TColor{100, 200, 100});

    TModel model("obj/african_head/source/african_head.obj");
    // TModel model("Z:/Develop/Models/obj/orc/source/model.obj");
    // TModel model("Z:/Develop/Models/obj/Lara/source/model.obj");
    // TModel model("Z:/Develop/Models/obj/tennyson/source/model.obj");
    // TModel model("Z:/Develop/Models/obj/mery/source/model.obj");
    // TModel model("Z:/Develop/Models/obj/airpods/source/model.obj");
    // TModel model("Z:/Develop/Models/obj/elizabeth/source/model.obj");

    BMP texture("obj/african_head/textures/african_head_diffuse.bmp");
    // BMP texture("obj/african_head/textures/african_head_nm_tangent.bmp");
    // BMP texture("obj/african_head/textures/african_head_spec.bmp");
    // BMP texture("Z:/Develop/Models/obj/orc/textures/Albedo.bmp");
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