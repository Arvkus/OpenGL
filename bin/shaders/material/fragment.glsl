#version 430 core

in vec3 normal;
in vec3 position;
in vec2 texture;

out vec4 FragColor;

void main(){
    //FragColor = vec4(normal, 1.0);
    float grey = (abs(normal.x) * abs(normal.y) * abs(normal.z)) + 0.5f;
    FragColor = vec4(grey, grey, grey, 1.0);
}