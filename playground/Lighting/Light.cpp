#include "Light.hpp"


Light::Light()
{
    m_color = glm::vec3(1,1,1);
    m_intensity = 1.0f;
}


Light::Light(glm::vec3 color, GLfloat intensity)
{
    m_color = color;
    m_intensity = intensity;
}


Light::~Light()
{

}

void Light::UseLight(GLfloat colorPropertyLocation, GLfloat intensityPropertyLocation)
{
    glUniform3f(colorPropertyLocation, m_color.x, m_color.y, m_color.z);
    glUniform1f(intensityPropertyLocation, m_intensity);
}