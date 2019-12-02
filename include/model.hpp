#pragma once
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <json/json.hpp>

#include "common.h"
#include "shaders.hpp"

//------------------------------------------------------------------------------

class Orientation{
private:
    glm::vec3 _position   = glm::vec3(0,0,0);
    glm::vec3 _size       = glm::vec3(1,1,1);
    glm::vec3 _rotation   = glm::vec3(0,0,0);
    glm::quat _quaternion = glm::quat(1,0,0,0);
    glm::mat4 _matrix = glm::mat4(1.0);

    /*
                     position ---\
                         size ---+-- cframe
        rotation - quaternion ---/
    */

    void construct_matrix(){
        // translate, rotate, scale
        glm::mat4 matrix = glm::mat4(1.0);
        matrix = glm::translate(matrix, _position); // translate
        matrix = matrix * glm::toMat4(_quaternion); // rotate
        matrix = glm::scale(matrix, _size); // scale
        this->_matrix = matrix;
    }

    void deconstruct_matrix(){
        glm::mat4 m = this->_matrix;

        float size_x = glm::sqrt( m[0][0]*m[0][0] + m[1][0]*m[1][0] + m[2][0]*m[2][0]); // length of X vector
        float size_y = glm::sqrt( m[0][1]*m[0][1] + m[1][1]*m[1][1] + m[2][1]*m[2][1]); // length of Y vector
        float size_z = glm::sqrt( m[0][2]*m[0][2] + m[1][2]*m[1][2] + m[2][2]*m[2][2]); // length of Z vector
        glm::vec3 size = glm::vec3(size_x, size_y, size_z);

        this->_size = size;
        this->_position = glm::vec3(m[3][0], m[3][1], m[3][2]);
        std::cout<<"deconstructed: ";
        printm(_position);

        // normalize rotaions
        m[0][0] /= size_x;
        m[1][0] /= size_x;
        m[2][0] /= size_x;

        m[0][1] /= size_y;
        m[1][1] /= size_y;
        m[2][1] /= size_y;

        m[0][2] /= size_z;
        m[1][2] /= size_z;
        m[2][2] /= size_z;

        this->_quaternion = glm::quat_cast(m);
        glm::vec3 angles = glm::eulerAngles(this->_quaternion);
        this->_rotation = glm::vec3( glm::degrees(angles.x), glm::degrees(angles.y), glm::degrees(angles.z) );
    }

public:

    glm::mat4 matrix(){ return this->_matrix; }
    glm::mat4 matrix(glm::mat4 m){ 
        this->_matrix = m;
        deconstruct_matrix();
        return m;
    }

    glm::vec3 position(){ return this->_position; }
    glm::vec3 position(glm::vec3 v){
        this->_position = v;
        construct_matrix();
        return v;
    }

    glm::vec3 rotation(){ return this->_rotation; }
    glm::vec3 rotation(glm::vec3 v){
        this->_rotation = v;
        this->_quaternion = glm::quat(glm::vec3(glm::radians(v.x), glm::radians(v.y), glm::radians(v.z)));
        construct_matrix();
        return v;
    }

    glm::vec3 size(){ return this->_size; }
    glm::vec3 size(glm::vec3 v){
        this->_size = v;
        construct_matrix();
        return v;
    }

    glm::quat quaternion(){ return this->_quaternion; }
    glm::quat quaternion(glm::quat v){
        this->_quaternion = v;
        construct_matrix();
        return v;
    }

};

//------------------------------------------------------------------------------

class Texture{
public:
    unsigned int id;
    TextureType type;
    const char *image_binary;
};

//metallic-roughness material model
// base color / metal / rough / emissive / occlusion / normal
// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#materials
class Material{ // TODO: define default values
public:
    std::string name;
    bool double_sided = true;

    glm::vec4 base_color_factor = glm::vec4(0.0, 0.0, 0.0, 1.0);
    float metalic_factor = 1.0;
    float roughness_factor = 0.0;

    std::vector<Texture> textures;

    // move textures to vector?
    /*
    Texture base_color_texture; // factor * texture * vertex = color
    Texture metallic_roughness_texture; // if null, 1.0
    Texture normal_texture;
    */

    // bidirectional reflectance distribution function (BRDF)  ????
    
    // normalTexture (scale index, texcoord)
    // occlusionTexture
    // emissiveTexture
    // emissive factor

};

//------------------------------------------------------------------------------

class Mesh : public Orientation{
private:
    unsigned int VAO, VBO, EBO;

public:
    std::string name;

    std::vector<Vertex> vertices;
    std::vector<unsigned short> indices;
    
    Material material;
    //std::vector<Texture> textures;

    std::vector<Mesh> children;

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


        for(Texture tex : material.textures){
            std::cout<<tex.type<<std::endl;
        }
    }

    void draw(Shader_program *sp, glm::mat4 parent_cframe){
        glBindVertexArray(VAO);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);

        for(Mesh mesh : children){

            glm::mat4 relative_cframe = parent_cframe * mesh.matrix();

            sp->set_uniform("model", relative_cframe );
            mesh.draw(sp, relative_cframe );
        }
    }
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class Model : public Orientation{

public:

    std::string name;
    std::vector<Mesh> meshes;

    void draw(Shader_program *sp){
        for(Mesh mesh : meshes){
            
            // relative position (matrix) to model
            glm::mat4 relative_cframe = this->matrix() * mesh.matrix();


            sp->set_uniform("model", relative_cframe );
            mesh.draw(sp, relative_cframe );
        }
    }

    Model(){
        //load_from_glb(path);
    }

};

//------------------------------------------------------------------------------

class Camera { // fix eye and world space camera matrices
private:

public:

    glm::mat4 matrix(){
        glm::vec3 pos = glm::vec3(0.0);
        pos.x = cos(glm::radians((float)yaw)) * cos(glm::radians((float)pitch));
        pos.y = sin(glm::radians((float)yaw));
        pos.z = cos(glm::radians((float)yaw)) * sin(glm::radians((float)pitch));
        return glm::inverse( glm::lookAt( origin + pos*distance, origin, glm::vec3(0,1,0) ) );
    }

    glm::mat4 view(){
        glm::vec3 pos = glm::vec3(0.0);
        pos.x = cos(glm::radians((float)yaw)) * cos(glm::radians((float)pitch));
        pos.y = sin(glm::radians((float)yaw));
        pos.z = cos(glm::radians((float)yaw)) * sin(glm::radians((float)pitch));
        return glm::lookAt( origin + pos*distance, origin, glm::vec3(0,1,0) );
    }

    glm::vec3 origin = glm::vec3(0.0); 
    float speed = 0;
    float distance = 30;
    int pitch = -45; // vertical (rotate on X)
    int yaw   = 60; // horizontal (rotate on Y)
    int roll  = 0;

    void move(Input input){
        //-----------------------------------------------------
        float x = cos(glm::radians((float)pitch));
        float y = sin(glm::radians((float)pitch));

        this->origin.x -= x * (input.w - input.s)*speed;
        this->origin.z -= y * (input.w - input.s)*speed;

        this->origin.x -= y * (input.a - input.d)*speed;
        this->origin.z += x * (input.a - input.d)*speed;
        //-----------------------------------------------------

        if(input.hold_middle){
            float x = input.mouse_delta.x;
            float y = input.mouse_delta.y;

            pitch = (pitch + x );
            yaw = (yaw + y );

            if(yaw >  80) yaw =  80;
            if(yaw < -80) yaw = -80; 
        }

        distance -= (distance - input.mouse_wheel) <= 3.0? 0.0: input.mouse_wheel;
    }
};