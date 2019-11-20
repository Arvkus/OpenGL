// Renderer: window creation, event handle, main loop

#pragma once
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <output.hpp>

class Canvas{
private:
    //----------------------------------------------------------------------
    SDL_GLContext context;
    SDL_Window *window;
    SDL_Event event;

    //----------------------------------------------------------------------
    void draw_objects(){
        
    }


public:
    //----------------------------------------------------------------------
    Canvas(){ // window & context creation
        int window_x = 640;
        int window_y = 480;

        // https://wiki.libsdl.org/SDL_Init#Remarks
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

        // https://wiki.libsdl.org/SDL_GL_SetAttribute#Remarks
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // CORE or ES or COMPABILITY
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // default 16 bits // ???

        window = SDL_CreateWindow("Object Loader", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,window_x,window_y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if(!window) printf("Window was not created: %s\n", SDL_GetError());
        context = SDL_GL_CreateContext(window); // openGL container/renderer

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) printf("Failed to initialize GLAD\n");

        SDL_GL_SetSwapInterval(1); // turn on vsync
        glEnable(GL_DEPTH_TEST); 
        glViewport(0,0,640,480);
        
        
        // successfuly created a window
        printf("Program start! | %s | %s\n",glGetString(GL_VERSION),glGetString(GL_RENDERER)); 
    }

    //----------------------------------------------------------------------
    // return "true" if QUIT
    bool handle_events(){
        SDL_PollEvent(&event);

        // get clicks // move user input to display class

        return event.type == SDL_QUIT;
    }

    //----------------------------------------------------------------------
    void handle_draw(){
        glClearColor(0 , .6 , .5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
    }

    //----------------------------------------------------------------------
};