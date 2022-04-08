#include "model.hpp"

int main() {
    const unsigned long width = 1000;
    const unsigned long height = 1000;
    BMP image(width, height);
    TModel model("obj/african_head/african_head.obj");
    // TModel model("obj/diablo3_pose/diablo3_pose.obj");
    // TModel model("obj/boggie/body.obj");
    // model.drawMesh(image);
    model.drawMeshTriangle(image);
    BMP zbf(width, height);
    model.drawZ_buffer(zbf);
    image.writeToFile("output.bmp");
    zbf.writeToFile("zbf.bmp");
    return 0;
}