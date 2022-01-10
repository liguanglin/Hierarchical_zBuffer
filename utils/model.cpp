#include<model.h>

void ObjData::setT(float _T[][4])
{
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            T[i][j] = _T[i][j];
        }
    }
}

point3f ObjData::getVertex(int idx){
    float x[4] = {vertex[idx].x,vertex[idx].y,vertex[idx].z,1}, y[4];
    for(int i=0; i<4; i++){
        float sum = 0;
        for(int j=0; j<4; j++){
            sum += T[i][j]*x[j];
        }
        y[i] = sum;
    }
    return point3f(y[0]/y[3], y[1]/y[3], y[2]/y[3]);
}

ObjData::ObjData(std::string fileName)
{
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            T[i][j] = (i==j);
        }
    }
    std::ifstream objFile(fileName);
    if(!objFile.is_open()){
        std::cout<<"file path is empty.\n";
    }
    std::string line;
    char type[10], p[100];
    float x,y,z;
    while(std::getline(objFile, line)){
        sscanf(line.c_str(),"%s",type);
        if(strlen(type)==1 && type[0] == 'v'){
            sscanf(line.c_str(),"%s%f%f%f",type,&x,&y,&z);
            vertex.push_back(point3f(x,y,z));
        }else
        if(strlen(type)==2 && type[0] == 'v' && type[1]=='t'){
            sscanf(line.c_str(),"%s%f%f",type,&x,&y);
            uv.push_back(point2f(x,y));
        }else 
        if(strlen(type)==2 && type[0] == 'v' && type[1]=='n'){
            sscanf(line.c_str(),"%s%f%f%f",type,&x,&y,&z);
            vertex.push_back(point3f(x,y,z));
        }else
        if(strlen(type)==1 && type[0] == 'f'){
            int v[3]={-1,-1,-1};
            int b = 1;
            face3d face;
            while(sscanf(line.c_str()+b,"%s",p)==1){
                std::string num = "";
                int l=strlen(p), idx=0;
                p[l++] = '/';
                p[l] = 0;
                for(int i=0; i<l; i++){
                    if(p[i]=='/'){
                        v[idx] = std::atoi(num.c_str());
                        idx++;
                        num="";
                    }
                    num = num + p[i];
                }
                face.vec.push_back(vertex3d(v[0]-1,v[1]-1,v[2]-1));
                b+=l;
            }
            //if(mesh.size()<1000)
            mesh.push_back(face);
        }
    }
    std::cout<<"load "<<vertex.size()<<" vertex, "
                    <<normal.size()<<" normals, "
                    <<uv.size()<<" uvs, "
                    <<mesh.size()<<" faces.\n";
}