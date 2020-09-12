#include "PointLight.hpp"

PointLight::PointLight()
    : Light(glm::vec3(0,0,0),0, 1024, 1024)
{
    m_position = glm::vec3(0,0,0);
    m_lightColor = glm::vec3(1,1,1);
    m_constant = 1;
    m_linear = 0;
    m_exponential = 0;
}

PointLight::PointLight(glm::vec3 position,  glm::vec3 pointColor, GLfloat constant, GLfloat linear, GLfloat exponential)
    : Light(glm::vec3(0,0,0),0, 1024, 1024)
{
    m_position = position;
    m_lightColor = pointColor;
    m_constant = constant;
    m_linear = linear;
    m_exponential = exponential;

}

PointLight::~PointLight() = default;

void PointLight::UseLight(GLuint pointPositionPropLocation, GLuint pointColorPropLocation, 
                    GLuint pointConstPropLocation, GLuint pointLinPropLocation, GLuint pointExpPropLocation) const
{
    glUniform3f(pointPositionPropLocation, m_position.x, m_position.y, m_position.z);
    glUniform3f(pointColorPropLocation, m_lightColor.x, m_lightColor.y, m_lightColor.z);
    glUniform1f(pointConstPropLocation, m_constant);
    glUniform1f(pointLinPropLocation, m_linear);
    glUniform1f(pointExpPropLocation, m_exponential);
}
