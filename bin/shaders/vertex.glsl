#version 430 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texture;

uniform mat4 model;
uniform mat4 view; // view = camera
uniform mat4 projection;

out vec3 normal;
out vec3 position;
out vec2 texture;

void main(){
    normal = a_normal;
    position = a_position;
    texture = a_texture;
    
    //vertex_pos = vec3(model * vec4(a_position, 1.0)); // vertex position in world space

    float x = a_position.x;
    float y = a_position.y;
    float z = a_position.z;
    gl_Position = projection * view * model * vec4(x,y,z, 1.0);
    
}
