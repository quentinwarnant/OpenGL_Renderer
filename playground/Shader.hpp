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

    GLuint GetUniformModelLocation();
    GLuint GetUniformViewLocation();
    GLuint GetUniformProjectionLocation();
    GLuint GetUniformAmbientColorLocation();
    GLuint GetUniformAmbientIntensityLocation();
    

private:
    GLuint m_shaderProgramLocation;
    GLuint m_uniformModelLocation;
    GLuint m_uniformViewLocation;
    GLuint m_uniformProjectionID;
    GLuint m_uniformAmbientColorLocation;
    GLuint m_uniformAmbientIntensityLocation;


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