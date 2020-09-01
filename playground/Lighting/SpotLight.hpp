#pragma once

#include "PointLight.hpp"

class SpotLight : public PointLight
{
public:
    SpotLight();
    SpotLight(glm::vec3 position, glm::vec3 spotDirection, glm::vec3 spotColor, GLfloat constant, GLfloat linear, GLfloat exponential, GLfloat angle);
    
    ~SpotLight();

    void UseLight(GLuint positionPropLocation, GLuint directionPropLocation, GLuint colorPropLocation, 
                    GLuint constPropLocation, GLuint linPropLocation, GLuint expPropLocation, GLuint anglePropLocation);

private:
    GLfloat m_angle;
    GLfloat m_processedAngle; // optimisation for shader to not have to do some maths 
    glm::vec3 m_direction;

};