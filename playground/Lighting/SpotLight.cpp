#include "SpotLight.hpp"

SpotLight::SpotLight() : PointLight()
{
    m_direction = glm::vec3(0,-1,0);
    m_angle = 10;
    m_processedAngle = cosf(glm::radians(m_angle));
 }

SpotLight::SpotLight(glm::vec3 position, glm::vec3 spotDirection, glm::vec3 spotColor , GLfloat constant, GLfloat linear, GLfloat exponential, GLfloat angle)
    :PointLight(position, spotColor, constant, linear, exponential)
{
    m_direction = spotDirection;
    m_angle = angle;
    m_processedAngle = cosf(glm::radians(m_angle));
}


SpotLight::~SpotLight() = default;


void SpotLight::UseLight(GLuint positionPropLocation, GLuint directionPropLocation, GLuint colorPropLocation, 
                    GLuint constPropLocation, GLuint linPropLocation, GLuint expPropLocation, GLuint anglePropLocation)
{
    glUniform3f(positionPropLocation, m_position.x, m_position.y, m_position.z);
    glUniform3f(directionPropLocation, m_direction.x, m_direction.y, m_direction.z);
    glUniform3f(colorPropLocation, m_lightColor.x, m_lightColor.y, m_lightColor.z);
    glUniform1f(constPropLocation, m_constant);
    glUniform1f(linPropLocation, m_linear);
    glUniform1f(expPropLocation, m_exponential);
    glUniform1f(anglePropLocation, m_processedAngle);
}
