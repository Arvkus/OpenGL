#pragma once
#include <glad/glad.h>
#include <SDL2/SDL.h>

#include "common.h"
#include "shaders.hpp"
//------------------------------------------------------------------------------

class Texture{
    unsigned int id;
    std::string type;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class Mesh{
public:
    std::string name;
    glm::vec3 scale;
    glm::vec3 translation = glm::vec3(0.0);

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
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW); 

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

private:
    unsigned int VAO, VBO, EBO;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class Model{
public:
    std::string name;
    std::vector<Mesh> meshes;

    void draw(Shader_program sp){
        for(Mesh mesh : meshes){
            
            // glm::mat4 pos = glm::mat4(1.0);
            //pos = glm::translate(glm::mat4(1.0), mesh.translation);
            //sp.set_uniform("model", pos);
            mesh.draw();
        }
    }

    Model(){
        //load_from_glb(path);
    }







    

private: 
    //--------------------------------------------------------------------------


 
    



    //--------------------------------------------------------------------------


    //--------------------------------------------------------------------------

};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------