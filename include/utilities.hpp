#pragma once
#include <glad/glad.h>
#include "common.h"

//----------------------------------------------------------------------

struct Vertex{
    Vertex(glm::vec3 v, glm::vec3 n, glm::vec2 t){
        position = v;
        normal = n;
        texcoord = t;
    };
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoord;
};

//----------------------------------------------------------------------

struct Input{
    bool w = false;
    bool a = false;
    bool s = false;
    bool d = false;

    glm::vec2 mouse_delta = glm::vec2(0,0);
    glm::vec2 mouse_position = glm::vec2(0,0);

    int mouse_wheel = 0; // [-1;1]

    bool hold_left = false; 
    bool hold_middle = false;
    bool hold_right = false;

    bool click_left = false;
    bool click_middle = false;
    bool click_right = false;

    int num = '1';

};
//----------------------------------------------------------------------

enum TextureType{
    diffuse, // color
    normal,
    metallic,
    rougness,
    AO, // ambient occlusion

    bump, // ??
    albedo, // ??
};

//----------------------------------------------------------------------
// print vectors and matrices

void printm(glm::quat m){ std::cout<<m[0]<<" "<<m[1]<<" "<<m[2]<<" "<<m[3]<<std::endl; }
void printm(glm::vec4 m){ std::cout<<m[0]<<" "<<m[1]<<" "<<m[2]<<" "<<m[3]<<std::endl; }
void printm(glm::vec3 m){ std::cout<<m[0]<<" "<<m[1]<<" "<<m[2]<<std::endl; }
void printm(glm::vec2 m){ std::cout<<m[0]<<" "<<m[1]<<std::endl; }

void printm(glm::mat4 m){
    
    std::cout.precision(3);
    std::cout<<std::fixed;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            std::cout<<std::setw(7)<<m[j][i]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

void printm(glm::mat3 m){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            std::cout<<m[j][i]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

void printm(glm::mat2 m){
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 2; j++){
            std::cout<<m[j][i]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

//----------------------------------------------------------------------
// is not required

glm::quat euler_to_quat(glm::vec3 rot){
    rot = glm::vec3(glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z));
    glm::quat QuatAroundX = glm::quat(1.0, 0.0, 0.0, rot.x); 
    glm::quat QuatAroundY = glm::quat(0.0, 1.0, 0.0, rot.y);
    glm::quat QuatAroundZ = glm::quat(0.0, 0.0, 1.0, rot.z);
    return QuatAroundX * QuatAroundY * QuatAroundZ;
}

//----------------------------------------------------------------------
