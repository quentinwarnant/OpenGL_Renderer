#include <stdio.h>
#include <stdlib.h>
#include <cmath>

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ControlSystem.hpp"



glm::mat4 ProjectionMatrix;
glm::mat4 ViewMatrix;



glm::vec3 position = glm::vec3(0,0,5);
float horizontalAngle = 3.14f;
float verticalAngle = 0.0f;
float initialFOV = 45.0f;
float speed = 3.0f;
float mouseSpeed = 0.005f;

float m_minMouseMoveThreshold = 25.0f;

int windowSizeWidth;
int windowSizeHeight;
double mousePosX;
double mousePosY;


void computeMatricesFromInputs(GLFWwindow* window, float deltaTime)
{
    glfwGetCursorPos( window, &mousePosX, &mousePosY);

    glfwGetWindowSize(window, &windowSizeWidth, &windowSizeHeight);

    //glfwSetCursorPos(window, windowSizeWidth/2.0f, windowSizeHeight/2.0f);

    // Compute new orientation
    float diffHorizontal = float(windowSizeWidth/2.0f - mousePosX );
    float diffVertical =  float( windowSizeHeight/2.0f - mousePosY );

    if( abs(diffHorizontal) > m_minMouseMoveThreshold )
    {
        horizontalAngle += mouseSpeed * deltaTime * diffHorizontal;
    }

    if( abs(diffVertical) > m_minMouseMoveThreshold )
    {

        verticalAngle   += mouseSpeed * deltaTime * diffVertical ;
    }

   // fprintf( stderr, "diffHoritonztal %f , diffVertical %f", float(windowSizeWidth/2.0f - mousePosX ), float(windowSizeHeight/2.0f - mousePosY ) );
		

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

   

    // Right vector
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f/2.0f), // 90 degrees = PI/2 radiants
        0,
        cos(horizontalAngle - 3.14f/2.0f)
    );

    // Up vector : perpendicular to both direction and right
    glm::vec3 up = glm::cross( right, direction );

    // Move forward
    if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){

        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS)
    {
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS)
    {
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS)
    {
        position -= right * deltaTime * speed;
    }

    // Move forward
    if (glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS){

        position = glm::vec3(0,0,5);
        up = glm::vec3(0,1,0);
        direction = glm::vec3(0,0,0) - position;

        Reset(window);

    }

    float FoV = initialFOV;// - 5 * glfwGetMouseWheel(); depricated

    // Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(glm::radians(FoV), float(windowSizeWidth) / float(windowSizeHeight), 0.1f, 100.0f);
    // Camera matrix
    ViewMatrix       = glm::lookAt(
        position,           // Camera is here
        position+direction, // and looks here : at the same position, plus "direction"
        up                  // Head is up (set to 0,-1,0 to look upside-down)
    );

}

void Reset(GLFWwindow* window)
{
    glfwSetCursorPos( window, windowSizeWidth / 2, windowSizeHeight / 2);

    horizontalAngle = -3.14;
    verticalAngle = 0;

}

glm::mat4 getProjectionMatrix()
{
    return ProjectionMatrix;
}

glm::mat4 getViewMatrix()
{
    return ViewMatrix;
}
