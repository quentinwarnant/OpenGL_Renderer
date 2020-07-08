#pragma once

#include <GL/glew.h>

class Shader
{
public:
    Shader();
    ~Shader();

    void LoadShader(const char* pathVertexShader, const char* pathFragmentShader);

    void StartUseShader();
    void EndUseShader();

    GLuint GetUniformModelID();
    GLuint GetUniformProjectionID();

private:
    GLuint m_shaderProgramID;
    GLuint m_uniformModelID;
    GLuint m_uniformProjectionID;

    void UnloadShader();

};