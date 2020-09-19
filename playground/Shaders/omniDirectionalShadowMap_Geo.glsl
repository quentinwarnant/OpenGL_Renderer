#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out; // 18 = 6 (sides of cubemap) X 3 vertices per geometry (triangle)

uniform mat4 lightMatrices[6]; // one ViewProj Matrix for each direction in the cubemap

out vec4 FragPos;

void main()
{
    for(int face = 0; face < 6; face++)
    {
        gl_Layer = face; // Define which of the 6 cubemap texture to emit into
        for(int i = 0; i < 3; i++) // each vertex in triangle
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = lightMatrices[face] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}
