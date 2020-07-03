#ifndef CONTROLSYSTEM_HPP
#define CONTROLSYSTEM_HPP

void computeMatricesFromInputs(GLFWwindow* window, float deltaTime);
glm::mat4 getProjectionMatrix();
glm::mat4 getViewMatrix();

void Reset(GLFWwindow* window);

#endif