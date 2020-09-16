#version 330 core

in vec2 vUV;

out vec4 color;

// reason why this is automatically identified as the bound texture is because there's only ONE atm.
// If there were more than one texture we'd have to bind the uniform
uniform sampler2D mainTexSampler;

void main()
{
    color = texture(mainTexSampler, vUV);
    float d = color.r * 20;//> 0.02 ? 1 : 0;
    color.rgb = vec3(d,d,d);
}