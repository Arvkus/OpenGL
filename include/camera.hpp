#pragma once
#include "common.h"

class Camera{

private:

    // change position based on distance/origin/rotation
    glm::mat4 calculate_cframe(){ // TODO: broken
        glm::vec3 pos = glm::vec3(0.0);
 
        pos.x = cos(glm::radians((float)yaw)) * cos(glm::radians((float)pitch));
        pos.y = sin(glm::radians((float)yaw));
        pos.z = cos(glm::radians((float)yaw)) * sin(glm::radians((float)pitch));

        //pos = glm::vec3(0,0,10);

        //printm(pos*distance);
        //std::cout<<std::endl;
        return glm::lookAt(origin + pos*distance, origin, glm::vec3(0,1,0));
    }



public:
    glm::vec3 get_position(){ // TODO: broken
        /*
        glm::mat4 cframe = calculate_cframe();
        glm::vec4 pos = cframe * glm::vec4(1.0);
        return glm::vec3(pos.x, pos.y, pos.z);
        */

        glm::vec3 pos = glm::vec3(0.0);
        pos.x = cos(glm::radians((float)yaw)) * cos(glm::radians((float)pitch));
        pos.y = sin(glm::radians((float)yaw));
        pos.z = cos(glm::radians((float)yaw)) * sin(glm::radians((float)pitch));
        return pos*distance/2.0f;
    }
    //----------------------------------------------

    glm::vec3 origin = glm::vec3(0,0,0);

    float speed = .1;
    float distance = 10;
    int pitch = 45; // vertical (rotate on X)
    int yaw   = 45; // horizontal (rotate on Y)
    int roll  = 0;

    //----------------------------------------------

    //----------------------------------------------

    glm::mat4 cframe(){
        //std::cout<<pitch<< " " << yaw << " \n";
        return calculate_cframe();
    }

    void move(Input input){

        //glm::vec3 direction = glm::normalize( origin - get_position() );
        //printm(direction);
        //std::cout<<std::endl;

        //-----------------------------------------------------
        float x = cos(glm::radians((float)pitch));
        float y = sin(glm::radians((float)pitch));

        this->origin.x -= x * (input.w - input.s)*speed;
        this->origin.z -= y * (input.w - input.s)*speed;

        this->origin.x -= y * (input.a - input.d)*speed;
        this->origin.z += x * (input.a - input.d)*speed;
        //-----------------------------------------------------

        if(input.mouse_middle){
            float x = input.mouse_delta.x;
            float y = input.mouse_delta.y;

            pitch = (pitch + (int)x ) % 360;
            yaw = (yaw + (int)y );

            if(yaw >  80) yaw =  80;
            if(yaw < -80) yaw = -80; 
        }

        distance -= (distance - input.mouse_wheel) <= 3.0? 0.0: input.mouse_wheel;
    }

};