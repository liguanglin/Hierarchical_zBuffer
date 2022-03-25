#include "utils/model.h"
#include "utils/OctTree.h"
#include "utils/zBuffer.h"
#include "utils/Display.h"

int main(int argc, char* argv[])
{
    if(argc == 1){
        std::cout<<"please input obj path, like ./main ../bunny.obj\n";
        return 0;
    }
    glutInit(&argc, argv);
    std::cout<<std::string(argv[1])<<std::endl;
    ObjData* mesh = new ObjData(std::string(argv[1]));
    OctTree* tree = new OctTree(mesh);
    zBuffer buffer(800, 600);
    buffer.render(tree);
    Display display(&buffer);
    display.run();
    return 0;
}