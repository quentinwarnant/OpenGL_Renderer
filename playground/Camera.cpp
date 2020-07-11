#include "Camera.hpp"
#include <iostream>

Camera::Camera(glm::vec3 pos, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat moveSpeed, GLfloat turnSpeed)
{
    m_pos = pos;
    m_worldUp = m_up = up;
    m_forward = glm::vec3(0.0f, 0.0f, -1.0f);

    m_yaw = yaw;
    m_pitch = pitch;

    m_moveSpeed = moveSpeed;
    m_turnSpeed = turnSpeed;
}

Camera::~Camera()
{

}

void Camera::Update(GLfloat deltaTime, glm::vec2 moveInput, glm::vec2 turnAxesInput)
{
    int inverseOperator = -1; //inverse pitch direction

    //process turnAxisInput into Yaw & Pitch
    m_yaw += turnAxesInput.x * m_turnSpeed * deltaTime;
    m_pitch += inverseOperator * turnAxesInput.y * m_turnSpeed * deltaTime;

    //Clamp pitch
    if( m_pitch > 89) 
    {
        m_pitch = 89;
    }
    else if( m_pitch < -89)
    {
        m_pitch = -89;
    }


    //Caluclate camera axes
    m_forward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_forward.y = sin(glm::radians(m_pitch));
    m_forward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_forward = glm::normalize(m_forward);

    //calculate right based on world up, because we don't know what the actual up is yet
    m_right = glm::normalize( glm::cross( m_forward, m_worldUp));

    //calculate actual up based on two calculated vectors
    m_up = glm::normalize( glm::cross( m_right, m_forward));


    // Move based on newly calculated forward direction
    Move(deltaTime, m_forward * moveInput.x, m_right * moveInput.y );


}

void Camera::Move(GLfloat deltaTime, glm::vec3 directionForward, glm::vec3 directionRight)
{
    m_pos += directionForward * m_moveSpeed * deltaTime;
    m_pos += directionRight * m_moveSpeed * deltaTime;
}

glm::mat4 Camera::CalculateViewMatrix()
{
    return glm::lookAt(m_pos, m_pos + m_forward, m_up );
}