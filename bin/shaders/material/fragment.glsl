#version 430 core

in vec3 normal;
in vec3 position;
in vec2 texture;

out vec4 FragColor;

void main(){
    //FragColor = vec4(normal, 1.0);

    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}