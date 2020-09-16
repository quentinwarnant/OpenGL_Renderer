#version 330

layout (location = 0) in vec3 pos;
uniform mat4 Model;
uniform mat4 directionalLightSpaceTransform; // projection (ortho) * view
void main() {
    gl_Position = directionalLightSpaceTransform * Model * vec4(pos, 1.0);
}
