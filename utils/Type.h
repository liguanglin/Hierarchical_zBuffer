#pragma once

#include<vector>
#include<iostream>
#include<algorithm>

const float INF = 1e8;

struct point3f{
    float x,y,z;
    point3f(float _x,float _y,float _z){
        x=_x,y=_y,z=_z;
    }
};

struct point2f{
    float x,y;
    point2f(float _x,float _y){
        x=_x, y=_y;
    }
};

struct point2d{
    int x,y;
    point2d(int _x,int _y){
        x=_x, y=_y;
    }
};

struct vertex3d{
    int v,n,u;
    vertex3d(int _v,int _n,int _u){
        v=_v,n=_n,u=_u;
    }
};

struct boundingbox3f{
    float x0,x1,y0,y1,z0,z1;
    boundingbox3f(){
        x0 = y0 = z0 = INF;
        x1 = y1 = z1 = -INF;
    }
    boundingbox3f(float _x0, float _x1, float _y0, float _y1, float _z0, float _z1){
        x0=_x0;
        x1=_x1;
        y0=_y0;
        y1=_y1;
        z0=_z0;
        z1=_z1;
    }
    void update(point3f p){
        x0 = std::min(x0, p.x);
        x1 = std::max(x1, p.x);
        y0 = std::min(y0, p.y);
        y1 = std::max(y1, p.y);
        z0 = std::min(z0, p.z);
        z1 = std::max(z1, p.z);
    }
};

struct boundingbox2f{
    float x0,x1,y0,y1;
    boundingbox2f(){
        x0 = y0 = INF;
        x1 = y1 = -INF;
    }
    boundingbox2f(float _x0, float _x1, float _y0, float _y1){
        x0=_x0;
        x1=_x1;
        y0=_y0;
        y1=_y1;
    }
    void update(point2f p){
        x0 = std::min(x0, p.x);
        x1 = std::max(x1, p.x);
        y0 = std::min(y0, p.y);
        y1 = std::max(y1, p.y);
    }
};

struct boundingbox2d{
    int x0,x1,y0,y1;
    boundingbox2d(int _x0, int _x1, int _y0, int _y1){
        x0=_x0;
        x1=_x1;
        y0=_y0;
        y1=_y1;
    }
    void update(point2d p){
        x0 = std::min(x0, p.x);
        x1 = std::max(x1, p.x);
        y0 = std::min(y0, p.y);
        y1 = std::max(y1, p.y);
    }
};


struct face3d{
    std::vector<vertex3d> vec;
};

struct face_points{
    std::vector<point3f> vec;
    static bool comp_x(const point3f&a, const point3f&b){
        return a.x<b.x;
    }
    static bool comp_y(const point3f&a, const point3f&b){
        return a.y<b.y;
    }
    static bool comp_z(const point3f&a, const point3f&b){
        return a.z<b.z;
    }
    void sort_x(){
        std::sort(vec.begin(), vec.end(), comp_x);
    }
    void sort_y(){
        std::sort(vec.begin(), vec.end(), comp_y);
    }
    void sort_z(){
        std::sort(vec.begin(), vec.end(), comp_z);
    }
};

void printT(float T[][4]);
point3f transformPoint(float T[][4], point3f p);
