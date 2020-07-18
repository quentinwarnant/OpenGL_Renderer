#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
    Light();
    Light(glm::vec3 color, GLfloat intensity);
    ~Light();

    void UseLight(GLfloat colorPropertyLocation, GLfloat intensityPropertyLocation);

private:
    glm::vec3 m_color;
    GLfloat m_intensity;


};