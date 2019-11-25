// Renderer: window creation, event handle, main loop

#pragma once
#include <glad/glad.h>
#include <SDL2/SDL.h>

#include "common.h"

class Canvas{
private:
    //----------------------------------------------------------------------
    SDL_GLContext context;
    SDL_Window *window;
    SDL_Event event;

    glm::vec2 screen = glm::vec2(1280.0, 720.0);

    //----------------------------------------------------------------------

public:

    Input input;
    //----------------------------------------------------------------------
    float get_x(){ return this->screen.x; }
    float get_y(){ return this->screen.y; }
    //----------------------------------------------------------------------
    Canvas(){ // window & context creation

        // https://wiki.libsdl.org/SDL_Init#Remarks
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

        // https://wiki.libsdl.org/SDL_GL_SetAttribute#Remarks
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // CORE or ES or COMPABILITY
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // default 16 bits // ???

        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1); // anti-aliasing
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

        window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,screen.x,screen.y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if(!window) printf("Window was not created: %s\n", SDL_GetError());
        context = SDL_GL_CreateContext(window); // openGL container/renderer

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) printf("Failed to initialize GLAD\n");

        SDL_GL_SetSwapInterval(1); // turn on vsync
        glEnable(GL_DEPTH_TEST); // 3D
        glEnable(GL_MULTISAMPLE); // anti-aliasing
        glViewport(0,0,screen.x, screen.y);
        
        
        // successfuly created a window
        printf("Program start! | %s | %s\n",glGetString(GL_VERSION),glGetString(GL_RENDERER)); 
    }

    //----------------------------------------------------------------------
    // return "true" if QUIT
    bool handle_events(){
        
        this->input.mouse_delta.x = 0;
        this->input.mouse_delta.y = 0;
        this->input.mouse_wheel = 0;

        while(SDL_PollEvent(&event)){
            // get clicks // move user input to display class
            if(event.type == SDL_WINDOWEVENT){
                if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
                    this->screen.x = event.window.data1;
                    this->screen.y = event.window.data2;
                    glViewport(0,0,this->screen.x, this->screen.y);
                }   
                
            }

            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == 'w') this->input.w = true;
                if(event.key.keysym.sym == 'a') this->input.a = true;
                if(event.key.keysym.sym == 's') this->input.s = true;
                if(event.key.keysym.sym == 'd') this->input.d = true;
            }

            if(event.type == SDL_KEYUP){
                if(event.key.keysym.sym == 'w') this->input.w = false;
                if(event.key.keysym.sym == 'a') this->input.a = false;
                if(event.key.keysym.sym == 's') this->input.s = false;
                if(event.key.keysym.sym == 'd') this->input.d = false;
            }


            if(event.type == SDL_MOUSEMOTION){
                this->input.mouse_delta.x = event.motion.xrel;
                this->input.mouse_delta.y = event.motion.yrel;
            }


            if(event.type == SDL_MOUSEBUTTONDOWN){
                if(event.button.button == SDL_BUTTON_LEFT) this->input.mouse_left = true;
                if(event.button.button == SDL_BUTTON_MIDDLE) this->input.mouse_middle = true;
                if(event.button.button == SDL_BUTTON_RIGHT) this->input.mouse_right = true;
            }

            if(event.type == SDL_MOUSEBUTTONUP){
                if(event.button.button == SDL_BUTTON_LEFT) this->input.mouse_left = false;
                if(event.button.button == SDL_BUTTON_MIDDLE) this->input.mouse_middle = false;
                if(event.button.button == SDL_BUTTON_RIGHT) this->input.mouse_right = false;
            }

            if(event.type == SDL_MOUSEWHEEL){
                this->input.mouse_wheel = event.wheel.y;
            }
            
        }

        return event.type == SDL_QUIT;
    }

    //----------------------------------------------------------------------
    void swap_window(){
        SDL_GL_SwapWindow(window);
    }

    //----------------------------------------------------------------------
};