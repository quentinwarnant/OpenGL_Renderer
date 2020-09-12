#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <playground/Lighting/Light.hpp>

class LightDirectional : public Light
{
public:
    LightDirectional();
    LightDirectional(glm::vec3 ambientColor, GLfloat ambientIntensity, glm::vec3 directionalColor,
                     GLfloat directionalIntensity, glm::vec3 direction,
                     GLuint shadowMapWidth, GLuint shadowMapHeight);
    ~LightDirectional();

    virtual void UseLight(GLint ambientColorPropLocation, GLint ambientIntensityPropLocation,
                            GLint directionalColorPropLocation, GLint directionalIntensityPropLocation, 
                            GLint directionalLightDirPropLocation);

    glm::mat4 CalculateLightTransform();

private:
    glm::vec3 m_directionalColor;
    GLfloat m_directionalIntensity;

    glm::vec3 m_direction;

};