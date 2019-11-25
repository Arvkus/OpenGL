// Development of photorealistic vizualisation tool
// Fotorealistiniu vizualizaciju irankio kurimas

#define SDL_MAIN_HANDLED 
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <stb/stb_image.h>

#include "common.h"

#include "shaders.hpp"
#include "canvas.hpp"
#include "camera.hpp"
#include "loader.hpp"

// order: scale, rotate, translate

// 1. build mesh hierarchy in model
// 2. mesh scale and translation
// 3. mesh texture

using json = nlohmann::json;

json create_node(int id){
    return {
        {"id", id},
        {"children",json::array()}
    };
}

int main(int argc, char *argv[]) {
    json root = create_node(3);
    json arr = json::array();

    root["children"].push_back( create_node(5) );
    root["children"].push_back( create_node(7) );

    //std::cout<<root.dump(4);

    Canvas canvas = Canvas();
    Camera camera = Camera();
    Shader_program sp = Shader_program();

    Model axis = Loader::model_from_glb("models/axes.glb");
    Model cube = Loader::model_from_glb("models/complex.glb");
    cube.position = glm::vec3(0,0,10);

    
    printm(cube.cframe());
    for(Mesh mesh : cube.meshes){
        std::cout<<(mesh.name)<< std::endl;
        printm(mesh.cframe());
    }
    

    //----------------------------------------------
    glm::mat4 proj = glm::mat4(1.0);
    glm::mat4 view = glm::mat4(1.0); //glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -10.0)); 
    glm::mat4 model = glm::mat4(1.0);
    //----------------------------------------------

    while(true){
        // process input from output class

        // process calculations
        camera.move( canvas.input );
        view = camera.cframe();
        proj = glm::perspective(glm::radians(45.0f), canvas.get_x()/canvas.get_y(), 0.1f, 100.0f);

        // draw
        glClearColor(0 , .6 , .5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sp.use();
        sp.set_uniform("projection", proj);
        sp.set_uniform("view", view);
        sp.set_uniform("model", model);

        axis.draw(sp);
        cube.draw(sp);

        // poll events
        canvas.swap_window();
        if(canvas.handle_events()) break;
    }

    return 0;
}


