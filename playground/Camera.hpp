#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera(glm::vec3 pos, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat moveSpeed, GLfloat turnSpeed);
    ~Camera();

    void Update(GLfloat deltaTime, glm::vec2 moveInput, glm::vec2 turnAxesInput);
    glm::mat4 CalculateViewMatrix();
    glm::vec3 GetPos(){return m_pos;}
    glm::vec3 GetForward(){return m_forward;}

private: 
    glm::vec3 m_pos;
    
    glm::vec3 m_forward;
    glm::vec3 m_up;
    glm::vec3 m_right;

    glm::vec3 m_worldUp;

    GLfloat m_yaw; // left-right
    GLfloat m_pitch; // up-down

    GLfloat m_moveSpeed;
    GLfloat m_turnSpeed;

    void Move(GLfloat deltaTime, glm::vec3 directionForward, glm::vec3 directionRight);

};