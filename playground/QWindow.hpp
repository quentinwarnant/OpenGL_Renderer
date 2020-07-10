#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class QWindow
{
public:
    QWindow(GLuint windowWidth, GLuint windowHeight, const char* title);
    ~QWindow();

    int Init();

    GLuint GetWindowBufferWidth() { return m_windowBufferWidth;}
    GLuint GetWindowBufferHeight() { return m_windowBufferHeight;}

    bool ShouldClose() { return glfwGetKey(m_mainWindow, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(m_mainWindow); }
    void SwapBuffers() { glfwSwapBuffers(m_mainWindow);}

private:
    GLFWwindow* m_mainWindow;

    GLuint m_windowWidth, m_windowHeight;
    int m_windowBufferWidth, m_windowBufferHeight;

    const char* m_title;
};