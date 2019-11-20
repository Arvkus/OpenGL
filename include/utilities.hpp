#pragma once
#include <iostream>
#include <glm/glm.hpp>
//----------------------------------------------------------------------
// print vectors and matrices

void printm(glm::vec4 m){ std::cout<<m[0]<<" "<<m[1]<<" "<<m[2]<<" "<<m[3]<<std::endl; }
void printm(glm::vec3 m){ std::cout<<m[0]<<" "<<m[1]<<" "<<m[2]<<std::endl; }
void printm(glm::vec2 m){ std::cout<<m[0]<<" "<<m[1]<<std::endl; }

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

