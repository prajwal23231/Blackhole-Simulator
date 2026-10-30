#version 450 core 

layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;

void main(){
    gl_Position = projection * view * vec4(aPos,1.0);
    gl_PointSize = clamp(600.0 / gl_Position.w, 8.0, 20.0);
}