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

    void ReloadSources();

    GLuint GetUniformModelLocation();
    GLuint GetUniformViewLocation();
    GLuint GetUniformProjectionLocation();
    
    GLuint GetUniformAmbientColorLocation();
    GLuint GetUniformAmbientIntensityLocation();
    GLuint GetUniformDirectionalLightColorLocation();
    GLuint GetUniformDirectionalLightIntensityLocation();
    GLuint GetUniformDirectionalLightDirectionLocation();
    

private:
    const char* m_pathVertexShader;
    const char* m_pathFragmentShader;

    GLuint m_shaderProgramLocation;

    //coordinate system Matrices
    GLuint m_uniformModelLocation;
    GLuint m_uniformViewLocation;
    GLuint m_uniformProjectionID;

    //Lighting
    GLuint m_uniformAmbientColorLocation;
    GLuint m_uniformAmbientIntensityLocation;
    GLuint m_uniformDirectionalLightColorLocation;
    GLuint m_uniformDirectionalLightIntensityLocation;
    GLuint m_uniformDirectionalLightDirectionLocation;


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