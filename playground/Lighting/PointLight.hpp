#pragma once

#include <vector>
#include "Light.hpp"
#include "OmniShadowMap.hpp"

class PointLight : public Light
{
public:
    PointLight();
    PointLight(glm::vec3 position,  glm::vec3 pointColor, GLfloat constant, GLfloat linear, GLfloat exponential,
                GLuint shadowMapWidth, GLuint shadowMapHeight, GLfloat nearPlane, GLfloat farPlane);
    ~PointLight();

    void UseLight( GLuint pointPositionPropLocation, GLuint pointColorPropLocation, 
                    GLuint pointConstPropLocation, GLuint pointLinPropLocation, GLuint pointExpPropLocation) const;

    std::vector<glm::mat4> CalculateLightTransforms();
    glm::vec3 GetPos(){return m_position;}
    void SetPos(glm::vec3 pos);
    GLfloat GetFarPlane(){ return m_farPlane;}


protected:
    glm::vec3 m_position;
    glm::vec3 m_lightColor;
    GLfloat m_constant;
    GLfloat m_linear;
    GLfloat m_exponential;

    //omnidirectional shadowmap
    GLfloat m_farPlane;
};
