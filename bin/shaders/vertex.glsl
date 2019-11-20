#version 330 core
layout(location = 0) in vec3 a_position;
//layout(location = 1) in vec3 a_normal;
//layout(location = 2) in vec2 a_texture;

uniform mat4 model;
uniform mat4 view; // view = camera
uniform mat4 projection;

//out vec3 normal;

void main(){
    //normal = a_normal;
    //vertex_pos = vec3(model * vec4(a_position, 1.0)); // vertex position in world space
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    
}
