#version 330

in vec4 FragPos;
uniform vec3 lightPos;
uniform float farPlane;


void main()
{
    // distance to farplane (0-1 values)
    float distance = length(FragPos.xyz - lightPos) / farPlane;

    //set depth buffer value
    gl_FragDepth = distance;
}
