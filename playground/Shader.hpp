#pragma once

#include <exception>
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
    GLuint GetUniformViewID();
    GLuint GetUniformProjectionID();

private:
    GLuint m_shaderProgramID;
    GLuint m_uniformModelID;
    GLuint m_uniformViewID;
    GLuint m_uniformProjectionID;

    void UnloadShader();
};

class ShaderException : std::exception
{
public:
    ShaderException(const char* reason) { m_reason = reason;}

    virtual const char* what() const throw()
    {
        return m_reason;
    }

private:
    const char* m_reason;
};