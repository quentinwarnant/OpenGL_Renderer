#pragma once
#include "Light.hpp"

class PointLight : Light
{
public:
    PointLight();
    PointLight(glm::vec3 position,  glm::vec3 pointColor, GLfloat constant, GLfloat linear, GLfloat exponential);
    ~PointLight();

    void UseLight( GLint pointPositionPropLocation, GLint pointColorPropLocation, 
                    GLint pointConstPropLocation, GLint pointLinPropLocation, GLint pointExpPropLocation);

private:
    glm::vec3 m_position;
    glm::vec3 m_pointColor;
    GLfloat m_constant;
    GLfloat m_linear;
    GLfloat m_exponential;
};
