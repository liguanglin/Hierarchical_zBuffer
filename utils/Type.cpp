#include "Type.h"

point3f transformPoint(float T[][4], point3f p)  //transform point with transformation T
{
    float x[4] = {p.x, p.y, p.z, 1}, y[4];
    for(int i=0; i<4; i++){
        y[i] = 0;
        // std::cout<<x[i]<<" ";
        for(int j=0; j<4; j++){
            // std::cout<<T[i][j]<<" ";
            y[i] += T[i][j]*x[j];
        }
        // std::cout<<y[i]<<std::endl;
    }
    return point3f(y[0]/y[3], y[1]/y[3], y[2]/y[3]);
}

void printT(float T[][4])  //print for debug
{
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            std::cout<<T[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
}