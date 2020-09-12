#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "playground/Lighting/ShadowMap.h"

class Light
{
public:
    Light();
    Light(glm::vec3 ambientColor, GLfloat ambientIntensity, GLuint shadowMapWidth, GLuint shadowMapHeight); //makes the class abstract???
    ~Light();

    virtual void UseLight(GLint ambientColorPropLocation, GLint ambientIntensityPropLocation);

    ShadowMap* GetShadowMap() { return m_shadowMap;}

private:
    glm::vec3 m_ambientColor;
    GLfloat m_ambientIntensity;

protected:
    ShadowMap* m_shadowMap;
    glm::mat4 m_lightProj;
};