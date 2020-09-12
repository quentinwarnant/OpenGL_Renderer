#pragma once
#include "Light.hpp"

class PointLight : Light
{
public:
    PointLight();
    PointLight(glm::vec3 position,  glm::vec3 pointColor, GLfloat constant, GLfloat linear, GLfloat exponential);
    ~PointLight();

    void UseLight( GLuint pointPositionPropLocation, GLuint pointColorPropLocation, 
                    GLuint pointConstPropLocation, GLuint pointLinPropLocation, GLuint pointExpPropLocation) const;

protected:
    glm::vec3 m_position;
    glm::vec3 m_lightColor;
    GLfloat m_constant;
    GLfloat m_linear;
    GLfloat m_exponential;
};
