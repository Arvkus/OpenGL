#pragma once
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <json/json.hpp>

#include "common.h"
#include "shaders.hpp"

using json = nlohmann::json;
//------------------------------------------------------------------------------

class Orientation{
private:
    glm::mat4 _cframe = glm::mat4(1.0);

    void construct_cframe(){
        // scale, rotate, translate
        glm::mat4 cframe = glm::mat4(1.0);
        cframe = glm::scale(cframe, size);
        cframe = glm::rotate(cframe, glm::radians(rotation.x) ,glm::vec3(1,0,0) );
        cframe = glm::rotate(cframe, glm::radians(rotation.y) ,glm::vec3(0,1,0) );
        cframe = glm::rotate(cframe, glm::radians(rotation.z) ,glm::vec3(0,0,1) );
        cframe = glm::translate(cframe, position);
        this->_cframe = cframe;
    }

public:
    glm::vec3 position = glm::vec3(0,0,0);
    glm::vec3 rotation = glm::vec3(0,0,0);
    glm::vec3 size     = glm::vec3(1,1,1);
    
    glm::mat4 cframe(glm::mat4 m){ 
        this->_cframe = m;
        return m;
    }

    glm::mat4 cframe(){ 
        construct_cframe();
        return this->_cframe;
    }

};

//------------------------------------------------------------------------------

class Texture{
    unsigned int id;
    std::string type;
};

//------------------------------------------------------------------------------

class Mesh : public Orientation{
private:
    unsigned int VAO, VBO, EBO;

public:
    std::string name;

    std::vector<Vertex> vertices;
    std::vector<unsigned short> indices;
    std::vector<Texture> textures;

    void setupMesh(){ // fill openGL buffers
        glGenVertexArrays(1, &VAO); 
        glGenBuffers(1,&VBO); // vertex buffer object (vertices storage)
        glGenBuffers(1,&EBO); // element buffer object (indices storage)

        glBindVertexArray(VAO);

        // copy vertices to buffer for openGL to use
        glBindBuffer(GL_ARRAY_BUFFER, VBO); 
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // copy indices to buffer for openGL to use
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW); 

        // Tell VBO how to read data of VBO
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float) ));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6*sizeof(float) ));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }

    void draw(){
        glBindVertexArray(VAO);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);
    }
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class Model : public Orientation{

public:

    std::string name;
    std::vector<Mesh> meshes;

    void draw(Shader_program sp){
        for(Mesh mesh : meshes){

            sp.set_uniform("model", (this->cframe() * mesh.cframe()) );
            mesh.draw();
        }
    }

    Model(){
        //load_from_glb(path);
    }

};

//------------------------------------------------------------------------------

struct Node{

    Mesh *mesh;
    std::vector<struct Node> children;

};

//------------------------------------------------------------------------------