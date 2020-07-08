#pragma once

#include <GL/glew.h>

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void CreateMesh(GLfloat* vertices, unsigned int vertCount, unsigned int* indices, unsigned int indicesCount);
    void RenderMesh();
    void ClearMesh();


private:
    GLuint VAO, VBO, IBO;
    GLsizei indexCount;
};