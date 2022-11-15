#version 330 core 

layout(location=0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out float depth;

void main(){
    gl_Position = projection * view * model * vec4(aPos,1.0f);
    depth = gl_Position.z / gl_Position.w;
}