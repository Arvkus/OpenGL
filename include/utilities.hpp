#pragma once
#include "common.h"
//----------------------------------------------------------------------
// print vectors and matrices

void printm(glm::vec4 m){ std::cout<<m[0]<<" "<<m[1]<<" "<<m[2]<<" "<<m[3]; }
void printm(glm::vec3 m){ std::cout<<m[0]<<" "<<m[1]<<" "<<m[2]; }
void printm(glm::vec2 m){ std::cout<<m[0]<<" "<<m[1]; }

void printm(glm::mat4 m){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            std::cout<<m[j][i]<<" ";
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
    bool w = 0;
    bool a = 0;
    bool s = 0;
    bool d = 0;

    glm::vec2 mouse_delta = glm::vec2(0,0);
    int mouse_wheel = 0;
    bool mouse_left = 0;
    bool mouse_middle = 0;
    bool mouse_right = 0;
};

//----------------------------------------------------------------------

//----------------------------------------------------------------------
