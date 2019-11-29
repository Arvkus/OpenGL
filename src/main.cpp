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
// 6. fix camera



// 7. separate render loop / engine loop
// 8. vertex interpolation


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
    Shader_program sp = Shader_program();

    Model complex = Loader::model_from_glb("models/complex.glb");
    Model cursor = Loader::model_from_glb("models/cube.glb");
    Model cube = Loader::model_from_glb("models/car.glb");

    cube.size( glm::vec3(.2,.2,.2) );
    cursor.size( glm::vec3(.2,.2,.2) );
    complex.position( glm::vec3(0,0,10) );
    /*
    cube.size( glm::vec3(.6,1.5,.7) );
    cube.position( glm::vec3(0,0,0) ); 
    cube.rotation( glm::vec3(0,45,45) );
    */

    //----------------------------------------------
    glm::mat4 proj = glm::mat4(1.0);
    glm::mat4 view = glm::mat4(1.0); //glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -10.0)); 
    glm::mat4 model = glm::mat4(1.0);
    //----------------------------------------------

    glm::vec3 angle(0.0);

    Game game = Game(&cube, &(canvas.input));

    while(true){
        // process input from output class

        if (canvas.input.hold_left){
            glm::vec3 dir = get_mouse_direction(&canvas, proj, camera.view());
            
            glm::mat4 view = camera.matrix();
            glm::vec3 cam = glm::vec3( view[3][0], view[3][1], view[3][2] );

            float angle = glm::acos (glm::dot(dir, glm::vec3(0,-1,0)));
            float len = cam.y / glm::cos(angle);

            glm::vec3 pos = cam + dir*len;
            cursor.position( pos );
        }

        // process calculations

        //camera.move( canvas.input );
        view = camera.view();
        proj = glm::perspective(glm::radians(45.0f), canvas.get_x()/canvas.get_y(), 0.1f, 100.0f);

        complex.rotation( angle += glm::vec3(0,0,1) ); //= 

        
        game.drive();



        // draw
        glClearColor(0 , .6 , .5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sp.use();
        sp.set_uniform("projection", proj);
        sp.set_uniform("view", view);
        sp.set_uniform("model", model);

        cube.draw(sp);
        complex.draw(sp);
        cursor.draw(sp);


        // poll events
        canvas.swap_window();
        if(canvas.handle_events()) break;
    }

    return 0;
}


