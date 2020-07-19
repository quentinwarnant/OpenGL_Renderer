#include <playground/Lighting/LightDirectional.hpp>

LightDirectional::LightDirectional() : Light()
{
    m_directionalColor = glm::vec3(1.0f, 0.5f, 0.5f);
    m_directionalIntensity = 1.0f;
    m_direction = glm::vec3(0.0f,-1.0f,0.0f);
}

LightDirectional::LightDirectional(glm::vec3 ambientColor, GLfloat ambientIntensity, glm::vec3 directionalColor, GLfloat directionalIntensity, glm::vec3 direction) 
        : Light( ambientColor, ambientIntensity)
{
    m_directionalColor = directionalColor;
    m_directionalIntensity = directionalIntensity;

    m_direction = direction;
}

LightDirectional::~LightDirectional()
{

}

void LightDirectional::UseLight(GLint ambientColorPropLocation, GLint ambientIntensityPropLocation,
                                 GLint directionalColorPropLocation, GLint directionalIntensityPropLocation, 
                                 GLint directionalLightDirPropLocation)
{
    Light::UseLight(ambientColorPropLocation, ambientIntensityPropLocation);

    glUniform3f(directionalColorPropLocation, m_directionalColor.x, m_directionalColor.y, m_directionalColor.z);
    glUniform1f(directionalIntensityPropLocation, m_directionalIntensity);

    glUniform3f(directionalLightDirPropLocation, m_direction.x, m_direction.y, m_direction.z);
}
