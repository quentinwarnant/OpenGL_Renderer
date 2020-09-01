#include "PointLight.hpp"

PointLight::PointLight()
{
    m_position = glm::vec3(0,0,0);
    m_pointColor = glm::vec3(1,1,1);
    m_constant = 1;
    m_linear = 0;
    m_exponential = 0;
}

PointLight::PointLight(glm::vec3 position,  glm::vec3 pointColor, GLfloat constant, GLfloat linear, GLfloat exponential)
{
    m_position = position;
    m_pointColor = pointColor;
    m_constant = constant;
    m_linear = linear;
    m_exponential = exponential;
}

PointLight::~PointLight()
{

}

void PointLight::UseLight(GLint pointPositionPropLocation, GLint pointColorPropLocation, 
                    GLint pointConstPropLocation, GLint pointLinPropLocation, GLint pointExpPropLocation)
{
    glUniform3f(pointPositionPropLocation, m_position.x, m_position.y, m_position.z);
    glUniform3f(pointColorPropLocation, m_pointColor.x, m_pointColor.y, m_pointColor.z);
    glUniform1f(pointConstPropLocation, m_constant);
    glUniform1f(pointLinPropLocation, m_linear);
    glUniform1f(pointExpPropLocation, m_exponential);
}
