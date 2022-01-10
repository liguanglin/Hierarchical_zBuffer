#pragma once

#include<iostream>
#include<vector>
#include<fstream>
#include<cstring>
#include "Type.h"

struct ObjData
{
    std::vector<point3f> vertex,normal;
    std::vector<point2f> uv;
    std::vector<face3d> mesh;
    float T[4][4];
    ObjData(std::string fileName);
    void setT(float _T[][4]);
    point3f getVertex(int idx);
};