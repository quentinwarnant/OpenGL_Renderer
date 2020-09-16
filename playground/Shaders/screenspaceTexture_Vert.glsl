#version 330

layout (location = 0) in vec3 pos;
layout(location = 1) in vec2 vertexUV;
out vec2 vUV;

void main() {
    vec3 newPos = (pos *0.5) - vec3(0.9,0.5, 1.0) ;
    gl_Position = vec4(newPos, 1.0);
    vUV = vertexUV;

}
