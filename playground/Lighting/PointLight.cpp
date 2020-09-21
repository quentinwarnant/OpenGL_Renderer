#include "PointLight.hpp"
#include <glm/gtc/matrix_transform.hpp>

PointLight::PointLight()
    : Light(glm::vec3(0,0,0),0, 0, 0)
{
    m_position = glm::vec3(0,0,0);
    m_lightColor = glm::vec3(1,1,1);
    m_constant = 1;
    m_linear = 0;
    m_exponential = 0;
}

PointLight::PointLight(glm::vec3 position,  glm::vec3 pointColor, GLfloat constant, GLfloat linear, GLfloat exponential,
                       GLuint shadowMapWidth, GLuint shadowMapHeight, GLfloat nearPlane, GLfloat farPlane)
    : Light(glm::vec3(0,0,0),0, 0, 0)
{
    m_position = position;
    m_lightColor = pointColor;
    m_constant = constant;
    m_linear = linear;
    m_exponential = exponential;

    //Create omnidirectional shadowmap
    if( shadowMapWidth > 0 && shadowMapWidth > 0)
    {
        float aspectRatio = (float)shadowMapWidth / (float)shadowMapWidth;
        assert(aspectRatio == 1 && "shadowmap is not square");

        m_lightProj = glm::perspective(glm::radians(90.0f), aspectRatio, nearPlane, farPlane);
        m_farPlane = farPlane;

        m_shadowMap = new OmniShadowMap();
        m_shadowMap->Init(shadowMapWidth, shadowMapHeight);
    }

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

std::vector<glm::mat4> PointLight::CalculateLightTransforms()
{
    std::vector<glm::mat4> lightMatrices;
    /*
    glm::vec3 directions[] = {
            glm::vec3(1.0,0.0,0.0),
            glm::vec3(-1.0,0.0,0.0),
            glm::vec3(0.0,1.0,0.0),
            glm::vec3(0.0,-1.0,0.0),
            glm::vec3(0.0,0.0,1.0),
            glm::vec3(0.0,0.0,-1.0),
    };
    glm::vec3 up[] = {
            glm::vec3(0.0,-1.0,0.0), //Negative Y?
            glm::vec3(0.0,-1.0,0.0),
            glm::vec3(0.0,0.0,1.0),
            glm::vec3(0.0,0.0,-1.0),
            glm::vec3(0.0,-1.0,0.0),
            glm::vec3(0.0,-1.0,0.0),
    };

    for (int i = 0; i < 6; ++i)
    {
        lightMatrices.push_back(m_lightProj * glm::lookAt( m_position, m_position+directions[i], up[i] ));
    }

*/




lightMatrices.push_back(m_lightProj * 
                 glm::lookAt(m_position, m_position + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
lightMatrices.push_back(m_lightProj * 
                 glm::lookAt(m_position, m_position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
lightMatrices.push_back(m_lightProj * 
                 glm::lookAt(m_position, m_position + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
lightMatrices.push_back(m_lightProj * 
                 glm::lookAt(m_position, m_position + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
lightMatrices.push_back(m_lightProj * 
                 glm::lookAt(m_position, m_position + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
lightMatrices.push_back(m_lightProj * 
                 glm::lookAt(m_position, m_position + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));


   





    return lightMatrices;
}

void PointLight::SetPos(glm::vec3 pos)
{
    m_position = pos;
}