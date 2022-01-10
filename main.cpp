#include "utils/model.h"
#include "utils/OctTree.h"
#include "utils/zBuffer.h"
#include "utils/Display.h"

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    ObjData* mesh = new ObjData("../model.obj");
    OctTree* tree = new OctTree(mesh);
    zBuffer buffer(800, 600);
    buffer.render(tree);
    Display display(&buffer);
    display.run();
    return 0;
}