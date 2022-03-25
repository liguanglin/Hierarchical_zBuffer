#pragma once

#include <iostream>
#include "OctTree.h"
#include "assert.h"
#include <math.h>
#include <chrono>

struct bufferNode{     //buffer node in hierarchical_zbuffer
    float z;
    int x0,x1,y0,y1;
    bool isleaf;
    bufferNode* son[4];
    bufferNode* father;
    bufferNode* tmp;
    bufferNode(int _x0, int _x1, int _y0, int _y1){  //init bounding box
        isleaf = false;
        z = INF;
        x0 = _x0, x1 = _x1;
        y0 = _y0, y1 = _y1;
        father = NULL;
        for(int i=0; i<4; i++){
            son[i]=NULL;
        }
    }
    void updateZ(float _z){  //update the z depth value in the zbuffer
        bufferNode* node=this;
        if(node->z > _z){
            node->z = _z;
            node = node->father;
            while(node!=NULL){
                node->z=0;
                for(int i=0; i<4; i++){
                    if(node->son[i]){
                        node->z = std::max(node->z, node->son[i]->z);
                    }
                }
                node = node->father;
            }
        }
    }
    bool checkInBB(boundingbox2d bb){
        if(x0 < bb.x0) return false;
        if(x1 > bb.x1) return false;
        if(y0 < bb.y0) return false;
        if(y1 > bb.y1) return false;
        return true;
    }
    bool checkOverlap(boundingbox2d bb){
        if(x1 < bb.x0) return false;
        if(x0 > bb.x1) return false;
        if(y1 < bb.y0) return false;
        if(y0 > bb.y1) return false;
        return true;
    }
};

struct zBuffer{             //check comments in the .cpp file
    int width,height;
    int fx, fy, cx, cy;
    float sum_time;
    int sum_skip;
    int sum_render;
    float proj[4][4];
    float T[4][4];
    float proj_t[4][4];
    float viewPoint[4][4];
    OctTree* tree;
    bufferNode* bufferRoot;
    bufferNode*** bufferID;
    ObjData* renderingObj;
    zBuffer(int _width, int _height);
    void updateT(float dT[][4]);
    void render(OctTree* tree);
    int render(Node* node);
    void rerender();
    float getMaxZ(boundingbox2d bb, bufferNode* node);
    void update(face3d face);
    void init(int x0, int x1, int y0, int y1, bufferNode* &node);
    void check(bufferNode*node);
    boundingbox2f getBoundingBox(std::vector<vertex3d> &vec);
    float getZ(std::vector<vertex3d> &vec);
};