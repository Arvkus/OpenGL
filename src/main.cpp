// Development of photorealistic vizualisation tool
// Fotorealistiniu vizualizaciju irankio kurimas

#define SDL_MAIN_HANDLED 
#include <glad/glad.h>
#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb/stb_image.h>
#include <iostream>

#include "utilities.hpp"
#include "shaders.hpp"
#include "canvas.hpp"
#include "camera.hpp"

int main(int argc, char *argv[]) {
    Canvas canvas = Canvas();
    Shader_program sp = Shader_program();
    
    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  

    //------------------------------------------------------------------------

    unsigned int VAO; glGenVertexArrays(1, &VAO); 
    unsigned int VBO; glGenBuffers(1, &VBO); // vertex buffer object (vertices storage)
    unsigned int EBO; glGenBuffers(1, &EBO); // element buffer object (indices storage)

    //------------------------------------------------------------------------
    glBindVertexArray(VAO); // once VAO is bound it stores subsequent VBO calls

    // copy vertices to buffer for openGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // copy indices to buffer for openGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    // Tell VBO how to read data of VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    //------------------------------------------------------------------------
    int width = 640;
    int height = 480;

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -5.0)); 
    glm::mat4 model = glm::mat4(1.0);



    while(true){
        // process input from output class

        // process calculations
        //std::cout<<(float)SDL_GetTicks()/1000<<std::endl;

        model = glm::rotate(model, glm::radians(180.0f)/60, glm::vec3(1.0, 0.0, 0.2));

        // draw
        sp.use();
        sp.set_uniform("projection", proj);
        sp.set_uniform("view", view);
        sp.set_uniform("model", model);

        glBindVertexArray(VAO);
        canvas.handle_draw();

        // poll events
        if(canvas.handle_events()) break;
    }

    return 0;
}


