// Development of photorealistic vizualisation tool
// Fotorealistiniu vizualizaciju irankio kurimas
// minimum GLSL - 4.0

#define SDL_MAIN_HANDLED 
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <stb/stb_image.h>

#include "common.h"

#include "shaders.hpp"
#include "canvas.hpp"
#include "loader.hpp"
#include "model.hpp"
#include "game.hpp"
// matrix[ column ][ row ] // with j,i printing

// 3. mesh texture / material
// 5. deconstruct matrix bug?
// 6. camera lookAt in world space
// 7. model/mesh deletion
// 8. UI


// 7. separate render loop / engine loop
// 8. vertex interpolation

//https://github.com/if1live/cef-gl-example/blob/master/cefsimple/main.cpp
//https://github.com/andmcgregor/cefgui

glm::vec3 get_mouse_direction(Canvas *canvas, glm::mat4 proj, glm::mat4 view){
    // http://antongerdelan.net/opengl/raycasting.html
    glm::vec2 mouse = canvas->input.mouse_position;
    glm::vec2 size = canvas->get_screen_size();

    float x = (2.0f * mouse.x) / size.x - 1.0f;
    float y = 1.0f - (2.0f * mouse.y) / size.y;

    // normalised device coordinates
    glm::vec2 ndc = glm::vec2(x, y);
    
    glm::vec4 clip = glm::vec4(ndc.x, ndc.y, -1.0, 1.0);  // view ray

    glm::vec4 ray = glm::inverse(proj) * clip; // camera coordinate
    ray = glm::vec4(ray.x, ray.y, -1.0, 0.0);

    glm::vec3 dir = glm::inverse( view ) * ray;
    dir = glm::normalize(dir);
    return dir;
}

//----------------------------------------------------------------------

int main(int argc, char *argv[]) {

    Canvas canvas = Canvas();
    Camera camera = Camera();

    Shader_program sps[] = {
        Shader_program("shaders/normal/vertex.glsl", "shaders/normal/fragment.glsl"),
        Shader_program("shaders/material/vertex.glsl", "shaders/material/fragment.glsl"),
    };

    
    Model complex = Loader::model_from_glb("models/complex.glb");

    //----------------------------------------------
    glm::mat4 proj = glm::mat4(1.0);
    glm::mat4 view = glm::mat4(1.0); //glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -10.0)); 
    glm::mat4 model = glm::mat4(1.0);
    //----------------------------------------------

    glm::vec3 angle(0.0);


    while(true){
        // process input from output class

        // process calculations

        camera.move( canvas.input );
        view = camera.view();
        proj = glm::perspective(glm::radians(45.0f), canvas.get_x()/canvas.get_y(), 0.1f, 100.0f);

        complex.rotation( angle += glm::vec3(0,0,1) ); //= 

        // draw
        glClearColor(0 , .6 , .5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        
        int current_sp = (canvas.input.num - 1) % (sizeof(sps)/sizeof(sps[0]));
        sps[current_sp].use();
        sps[current_sp].set_uniform("projection", proj);
        sps[current_sp].set_uniform("view", view);
        sps[current_sp].set_uniform("model", model);
        sps[current_sp].set_uniform("eye", model);

        complex.draw(&sps[current_sp]);


        // poll events
        canvas.swap_window();
        if(canvas.handle_events()) break;
    }

    return 0;
}


