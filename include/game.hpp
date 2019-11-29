#include <glad/glad.h>
#include <SDL2/SDL.h>

#include "common.h"

#include "model.hpp"
#include "canvas.hpp"

class Game{
private: 

    float angle = 0;
    float speed = .2;
    Model *car;
    Input *input;

public:

    Game(Model *car, Input *input){
        this->car = car;
        this->input = input;
    }

    void drive(){
         angle += (input->a - input->d)*3;

        float x = cos(glm::radians(-angle)) * input->w * speed;
        float y = sin(glm::radians(-angle)) * input->w * speed;


        car->rotation( glm::vec3(0,angle,0) );
        car->position( car->position() + glm::vec3(x,0,y) );
    }



};