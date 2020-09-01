#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
    Light();
    Light(glm::vec3 ambientColor, GLfloat ambientIntensity); //makes the class abstract
    ~Light();

    virtual void UseLight(GLint ambientColorPropLocation, GLint ambientIntensityPropLocation);

private:
    glm::vec3 m_ambientColor;
    GLfloat m_ambientIntensity;
};