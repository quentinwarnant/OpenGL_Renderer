#include "Light.hpp"


Light::Light()
{
    m_ambientColor = glm::vec3(1,1,1);
    m_ambientIntensity = 1.0f;
}


Light::Light(glm::vec3 ambientColor, GLfloat ambientIntensity, GLuint shadowMapWidth, GLuint shadowMapHeight)
{
    m_ambientColor = ambientColor;
    m_ambientIntensity = ambientIntensity;

    if( shadowMapWidth > 0 && shadowMapHeight > 0)
    {
        m_shadowMap = new ShadowMap();
        m_shadowMap->Init(shadowMapWidth, shadowMapHeight);
    }
}


Light::~Light()
{
    if( m_shadowMap != nullptr ){
        delete m_shadowMap;
        m_shadowMap = nullptr;
    }
}

void Light::UseLight(GLint ambientColorPropLocation, GLint ambientIntensityPropLocation)
{
    glUniform3f(ambientColorPropLocation, m_ambientColor.x, m_ambientColor.y, m_ambientColor.z);
    glUniform1f(ambientIntensityPropLocation, m_ambientIntensity);
}