#pragma once

#include <GL/glew.h>

class Material
{
public:
    Material(GLfloat specularIntensity, GLfloat shininess);
    ~Material();

    void UseMaterial(GLuint specIntensityLocation, GLuint shininessLocation);

private:

    GLfloat m_specularIntensity;
    GLfloat m_shininess; //specular power - the higher the smoother the material

};