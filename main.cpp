#include "model.hpp"

int main() {
    const unsigned long width  = 1000;
    const unsigned long height = 1000;
    BMP output(width, height, TColor{0, 0, 0});

    TModel model("obj/african_head/african_head.obj");
    // TModel model("obj/diablo3_pose/diablo3_pose.obj");
    // TModel model("obj/boggie/body.obj");
    // TModel model("test.obj");

    BMP texture("obj/african_head/african_head_diffuse.bmp");
    // BMP texture("obj/diablo3_pose/diablo3_pose_diffuse.bmp");
    // BMP texture("texture.bmp");

    // model.drawMesh(output);
    // model.drawMeshTriangle(output);
    // model.draw_UV_map(output);
    model.drawMeshTexture(output, texture);

    output.writeToFile("output.bmp");

    // BMP zbf(width, height);
    // model.drawZ_buffer(zbf);
    // zbf.writeToFile("zbf.bmp");

    return 0;
}