#include "OctTree.h"
#include <assert.h>

void print_bound(float **axis_bound)
{
    for(int i=0; i<3; i++){
        std::cout<<axis_bound[i][0]<<" "<<axis_bound[i][1]<<std::endl;
    }
}

bool OctTree::check_bound(face3d face, boundingbox3f bb)
{
    boundingbox3f face_bb;
    for(int i=0; i<face.vec.size(); i++){
        point3f p = obj->vertex[face.vec[i].v];
        face_bb.update(p);
    }
    if(face_bb.x0 < bb.x0 || face_bb.x1 > bb.x1) return false;
    if(face_bb.y0 < bb.y0 || face_bb.y1 > bb.y1) return false;
    if(face_bb.z0 < bb.z0 || face_bb.z1 > bb.z1) return false;
    return true;
}

void OctTree::update(int face_id)
{
    update(face_id, bb, root);
}

void OctTree::update(int face_id, boundingbox3f bb, Node* &node)
{
    if(node == NULL){
        cnt++;
        node = new Node(bb);
    }
    node->faceNum++;
    float axis[3][3]={
        bb.x0, (bb.x0+bb.x1)/2, bb.x1,
        bb.y0, (bb.y0+bb.y1)/2, bb.y1,
        bb.z0, (bb.z0+bb.z1)/2, bb.z1
    };

    int substree_num = 0;
    for(int i=0; i<2; i++)
    for(int j=0; j<2; j++)
    for(int k=0; k<2; k++){
        boundingbox3f sub_bb(axis[0][i],  axis[0][i+1], axis[1][j],
                             axis[1][j+1],axis[2][k],   axis[2][k+1]);

        if(check_bound(obj->mesh[face_id], sub_bb)){
            update(face_id, sub_bb, node->son[substree_num]);
            assert(node->son[substree_num] != NULL);
            return;
        }
        substree_num ++;
    }
    node->addFace(face_id);
}

OctTree::OctTree(ObjData *_obj)
{
    root = NULL;
    obj = _obj;
    for(int i=0; i<obj->vertex.size(); i++){
        bb.update(obj->vertex[i]);
    }
    for(int i=0; i<obj->mesh.size(); i++){
        update(i);
    }
    std::cout<<root->bb.x0<<" "<<root->bb.x1<<std::endl;
    std::cout<<"built OctTree with "<<cnt<<" nodes"<<std::endl;
}