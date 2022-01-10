#pragma once

#include<model.h>
#include<iostream>
#include "Type.h"

struct Node{
    std::vector<int> faces;
    int faceNum;
    boundingbox3f bb;
    Node* son[8];
    Node(boundingbox3f _bb){
        faceNum = 0;
        bb = _bb;
        for(int i=0; i<8; i++){
            son[i] = NULL;
        }
    }
    void addFace(int face_id){
        faces.push_back(face_id);
    }
    boundingbox3f getBoundingBox(float T[][4]){
        float v[3][2] = {
            bb.x0, bb.x1,
            bb.y0, bb.y1,
            bb.z0, bb.z1
        };
        boundingbox3f bb_T;
        for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
        for(int k=0; k<2; k++){
            point3f p(v[0][i], v[1][j], v[2][k]);
            p = transformPoint(T, p);
            bb_T.update(p);
        }
        return bb_T;
    }
};

struct OctTree{
    ObjData* obj;
    Node* root;
    boundingbox3f bb;
    int cnt = 0;
    int dims = 3;
    OctTree(ObjData* _obj);
    void update(int face_id, boundingbox3f bb, Node* &node);
    bool check_bound(face3d face, boundingbox3f bb);
    void update(int face_id);
};