#include "zBuffer.h"
#include "assert.h"

void zBuffer::check(bufferNode* node)
{
    if(node == NULL){
        return;
    }
    for(int i=0; i<4; i++){
        if(node->son[i]){
            assert(node->son[i]->father == node);
            check(node->son[i]);
        }
    }
}

zBuffer::zBuffer(int _width, int _height)
{
    width = _width;
    height = _height;

    fx = width;
    fy = height;

    cx = width/2;
    cy = height/2;

    bufferRoot = NULL;

    float zNear = 0.2, zFar = 3, aspect = 1.0*width/height, fov = acos(-1)/2;
    float zRange = zNear - zFar, tanHFov = tanf(fov/2);

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            proj[i][j] = (i==j);
            T[i][j] = (i==j);
            viewPoint[i][j] = (i==j);
        }
    }

    proj[0][0] = 1.0f / (tanHFov*aspect), proj[0][1] = 0.0f, proj[0][2] = 0.0f, proj[0][3] = 0.0;
    proj[1][0] = 0.0f, proj[1][1] = 1.0f/tanHFov, proj[1][2] = 0.0f, proj[1][3] = 0.0;
    proj[2][0] = 0.0f, proj[2][1] = 0.0f, proj[2][2] = (-zNear - zFar) / zRange, proj[2][3] = 2.0f * zFar*zNear / zRange;
    proj[3][0] = 0.0f, proj[3][1] = 0.0f, proj[3][2] = 1.0f, proj[3][3] = 0.0f;

    viewPoint[2][3] = 1;

    float tmp[4][4];
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            tmp[i][j] = 0;
            for(int k=0; k<4; k++){
                tmp[i][j] += proj[i][k]*viewPoint[k][j];
            }
        }
    }

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            proj[i][j] = tmp[i][j];
        }
    }

    bufferID = new bufferNode**[width];
    for(int i=0; i<width; i++){
        bufferID[i] = new bufferNode*[height];
    }
    std::cout<<"begin init\n";
    init(1, width, 1, height, bufferRoot);
    check(bufferRoot);
    std::cout<<"initailized buffer with size "<<width<<" "<<height<<std::endl;
}

void zBuffer::updateT(float dT[][4])
{
    float tmp[4][4];
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            tmp[i][j] = 0;
            for(int k=0; k<4; k++){
                tmp[i][j] += dT[i][k] * T[k][j];
            }
        }
    }
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            T[i][j] = tmp[i][j];
        }
    }
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            tmp[i][j] = 0;
            for(int k=0; k<4; k++){
                tmp[i][j] += proj[i][k]*T[k][j];
            }
        }
    }
    // printT(tmp);
    renderingObj->setT(tmp);
}

float zBuffer::getMaxZ(boundingbox2d bb, bufferNode* node)
{
    if(node == NULL){
        return 0;
    }
    if(!node->checkOverlap(bb)){
        return 0;
    }
    if(node->checkInBB(bb)){
        return node->z;
    }
    if(node->father) assert(node->z >= node->father->z);
    float max_z = 0;
    for(int i=0; i<4; i++){
        max_z = std::max(max_z, getMaxZ(bb, node->son[i]));
    }
    return max_z;
}

void zBuffer::init(int x0, int x1, int y0, int y1, bufferNode* &node)
{
    if(x0>x1 || y0>y1){
        return;
    }
    if(node == NULL)
        node = new bufferNode(x0-1, x1-1, y0-1, y1-1);
    node->z = INF;
    if(x0==x1 && y0==y1){
        node->isleaf = true;
        bufferID[x0-1][y0-1] = node;
        return;
    }
    int x_mid = (x0+x1)/2, y_mid = (y0+y1)/2;
    init(x0,      x_mid, y0,      y_mid, node->son[0]);
    init(x0,      x_mid, y_mid+1, y1,    node->son[1]);
    init(x_mid+1, x1,    y0,      y_mid, node->son[2]);
    init(x_mid+1, x1,    y_mid+1, y1,    node->son[3]);
    for(int i=0; i<4; i++){
        if(node->son[i] != NULL){
            node->son[i]->father = node;
            node->son[i]->tmp = node;
        }
    }
}

void zBuffer::update(face3d face)
{
    face_points points;
    for(int i=0; i<face.vec.size(); i++){
        point3f p = renderingObj->getVertex(face.vec[i].v);
        p.x = p.x*fx + cx;
        p.y = p.y*fy + cy;
        assert(p.z < 1);
        points.vec.push_back(p);
    }
    points.sort_x();
    // for(int i=0; i<3; i++){
    //     std::cout<<points.vec[i].x<<" "<<points.vec[i].y<<" "<<points.vec[i].z<<std::endl;
    // }
    point3f v1=points.vec[0], v2=points.vec[1], v3=points.vec[2];
    int x1 = v1.x+0.5, x2 = v2.x, x3 = v3.x;
    int y_max = std::min(std::max(std::max(v1.y, v2.y), v3.y), height-1.0f);
    int y_min = std::max(std::min(std::min(v1.y, v2.y), v3.y), 0.0f);
    if(x1 == x2 && x2 == x3){
        // std::cout<<"done\n";
        return ;
    }
    if(x1 == x2){
        float dy31 = (v3.y - v1.y)/(v3.x - v1.x);
        float dy32 = (v3.y - v2.y)/(v3.x - v2.x);
        float dz31 = (v3.z - v1.z)/(v3.x - v1.x);
        float dz32 = (v3.z - v2.z)/(v3.x - v2.x);
        for(int x=std::max(x1,0); x<=x3&&x<width; x++){
            float y1 = v1.y + dy31*(x-v1.x);
            float y2 = v2.y + dy32*(x-v2.x);
            float z1 = v1.z + dz31*(x-v1.x);
            float z2 = v2.z + dz32*(x-v2.x);
            if((int)y1==(int)y2){
                if((int)y2<height && (int)y2>0)
                    bufferID[x][(int)y2]->updateZ(z1);
                continue;
            }
            float dzy = (z2 - z1)/(y2 - y1);
            int yl = y1, yr = y2;
            if(v1.y > v2.y) std::swap(yl, yr);
            yl = std::max(y_min, yl);
            yr = std::min(yr, y_max);
            for(int y=yl; y<=yr; y++){
                float z = z1 + dzy*(y-y1);
                bufferID[x][y]->updateZ(z);
            }
        }
    }else if(x2 == x3){
        float dy31 = (v3.y - v1.y)/(v3.x - v1.x);
        float dy21 = (v2.y - v1.y)/(v2.x - v1.x);
        float dz31 = (v3.z - v1.z)/(v3.x - v1.x);
        float dz21 = (v2.z - v1.z)/(v2.x - v1.x);
        for(int x=std::max(x1,0); x<=x3&&x<width; x++){
            float y2 = v1.y + dy21*(x-v1.x);
            float y3 = v1.y + dy31*(x-v1.x);
            float z2 = v1.z + dz21*(x-v1.x);
            float z3 = v1.z + dz31*(x-v1.x);
            if((int)y2==(int)y3){
                if((int)y2<height && (int)y2>0)
                    bufferID[x][(int)y2]->updateZ(z2);
                continue;
            }
            float dzy = (z2 - z3)/(y2 - y3);
            int yl = y2, yr = y3;
            if(yl > yr) std::swap(yl, yr);
            yl = std::max(y_min, yl);
            yr = std::min(yr, y_max);
            for(int y=yl; y<=yr; y++){
                float z = z3 + dzy*(y-y3);
                assert(bufferID[x][y]);
                bufferID[x][y]->updateZ(z);
            }
        }
    }else {
        float dy31 = (v3.y - v1.y)/(v3.x - v1.x);
        float dy21 = (v2.y - v1.y)/(v2.x - v1.x);
        float dz31 = (v3.z - v1.z)/(v3.x - v1.x);
        float dz21 = (v2.z - v1.z)/(v2.x - v1.x);
        for(int x=std::max(x1,0); x<=x2&&x<width; x++){
            float y2 = v1.y + dy21*(x-v1.x);
            float y3 = v1.y + dy31*(x-v1.x);
            float z2 = v1.z + dz21*(x-v1.x);
            float z3 = v1.z + dz31*(x-v1.x);
            if((int)y2==(int)y3){
                if((int)y2<height && (int)y2>0)
                    bufferID[x][(int)y2]->updateZ(z2);
                continue;
            }
            float dzy = (z2 - z3)/(y2 - y3);
            int yl = y2, yr = y3;
            if(yl > yr) std::swap(yl, yr);
            yl = std::max(y_min, yl);
            yr = std::min(yr, y_max);
            for(int y=yl; y<=yr; y++){
                float z = z3 + dzy*(y-y3);
                assert(bufferID[x][y]);
                bufferID[x][y]->updateZ(z);
            }
        }
        float dy32 = (v3.y - v2.y)/(v3.x - v2.x);
        float dz32 = (v3.z - v2.z)/(v3.x - v2.x);
        for(int x=std::max(x2,0)+1; x<=x3&&x<width; x++){
            float y1 = v1.y + dy31*(x-v1.x);
            float y2 = v2.y + dy32*(x-v2.x);
            float z1 = v1.z + dz31*(x-v1.x);
            float z2 = v2.z + dz32*(x-v2.x);
            if((int)y1==(int)y2){
                if((int)y2<height && (int)y2>0)
                    bufferID[x][(int)y2]->updateZ(z1);
                continue;
            }
            float dzy = (z2 - z1)/(y2 - y1);
            int yl = y1, yr = y2;
            if(yl > yr) std::swap(yl, yr);
            yl = std::max(y_min, yl);
            yr = std::min(yr, y_max);
            for(int y=yl; y<=yr; y++){
                float z = z1 + dzy*(y-y1);
                bufferID[x][y]->updateZ(z);
            }
        }
    }
    // std::cout<<"done\n";
}

int zBuffer::render(Node* node)
{
    boundingbox3f bb = node->getBoundingBox(renderingObj->T);
    boundingbox2d bb2d = boundingbox2d((int)(bb.x0*fx+cx), (int)(bb.x1*fx+cx), (int)(bb.y0*fy+cy), (int)(bb.y1*fy+cy));
    float buffer_max_z = this->getMaxZ(bb2d, bufferRoot);

    if(bb.z0 > buffer_max_z){
        return node->faceNum;
    }
    for(int i=0; i<node->faces.size(); i++){
        update(renderingObj->mesh[node->faces[i]]);
    }
    int sum = 0;
    for(int i=0; i<8; i++){
        if(node->son[i] != NULL){
            sum+=render(node->son[i]);
        }
    }
    return sum;
}

void zBuffer::rerender()
{
    init(1, width, 1, height, bufferRoot);
    auto begin = std::chrono::system_clock::now();
    int skip_num = render(tree->root);
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout<<"render finished, skip "<<skip_num<<" faces. time used "<<duration.count()<<" ms.\n";
}

void zBuffer::render(OctTree* _tree)
{
    tree = _tree;
    renderingObj = tree->obj;
    tree->obj->setT(proj);
    auto begin = std::chrono::system_clock::now();
    int skip_num = render(tree->root);
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout<<"render finished, skip "<<skip_num<<" faces. time used "<<duration.count()<<" ms.\n";
}