#pragma once
#include "Light.hpp"

class PointLight : Light //TODO: Currently not actually making use of this derivation (which is why it's actually private) - Revisit whether to store "color" inside base class, and have ambient be a special extra property of directional light
{
public:
    PointLight();
    PointLight(glm::vec3 position,  glm::vec3 pointColor, GLfloat constant, GLfloat linear, GLfloat exponential);
    ~PointLight();

    void UseLight( GLuint pointPositionPropLocation, GLuint pointColorPropLocation, 
                    GLuint pointConstPropLocation, GLuint pointLinPropLocation, GLuint pointExpPropLocation);

protected:
    glm::vec3 m_position;
    glm::vec3 m_lightColor;
    GLfloat m_constant;
    GLfloat m_linear;
    GLfloat m_exponential;
};
