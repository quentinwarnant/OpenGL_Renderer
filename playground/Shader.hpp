#pragma once

#include <exception>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Settings.hpp"
#include "Lighting/LightDirectional.hpp"
#include "Lighting/PointLight.hpp"
#include "Lighting/SpotLight.hpp"

class Shader
{
public:
    Shader();
    ~Shader();

    void LoadShader(const char* pathVertexShader, const char* pathFragmentShader, const char* pathGeometryShader = nullptr);

    void StartUseShader();
    void EndUseShader();

    void ReloadSources();

    void SetDirectionalLight(LightDirectional* light);
    void SetPointLights(PointLight* lights, GLuint pointLightCount);
    void SetSpotLights(SpotLight* lights, GLuint spotLightCount);
    void SetTexture(GLuint textureUnit);
    void SetDirectionalShadowMap(GLuint textureUnit);
    void SetDirectionalLightTransform(glm::mat4 *lightTransform);

    GLuint GetUniformModelLocation();
    GLuint GetUniformViewLocation();
    GLuint GetUniformProjectionLocation();

    GLuint GetUniformCameraWorldPosLocation();
    
    GLuint GetUniformAmbientColorLocation();
    GLuint GetUniformAmbientIntensityLocation();
    GLuint GetUniformDirectionalLightColorLocation();
    GLuint GetUniformDirectionalLightIntensityLocation();
    GLuint GetUniformDirectionalLightDirectionLocation();
    
    GLuint GetUniformSpecularIntensityLocation();
    GLuint GetUniformSpecularShininessLocation();

    //Omnidirectional shadowmap
    GLuint GetUniformFarPlaneLocation() const;
    GLuint GetUniformOmniLightPosLocation() const;
    void SetLightMatrices(std::vector<glm::mat4> lightMatrices);

private:
    const char* m_pathVertexShader;
    const char* m_pathFragmentShader;
    const char* m_pathGeometryShader;

    GLuint m_shaderProgramLocation;

    //coordinate system Matrices
    GLuint m_uniformModelLocation;
    GLuint m_uniformViewLocation;
    GLuint m_uniformProjectionLocation;

    //?
    GLuint m_uniformCameraWorldPosLocation;

    //Lighting

    struct {
      GLuint ambientColor;
      GLuint ambientIntensity;
      GLuint directionalColor;
      GLuint directionalColorIntensity;
      GLuint direction;

    } m_uniformDirectionaLightLocations;

    //Current amount of point lights set
    GLuint m_pointLightCount;
    GLuint m_uniformPointLightCount;

    struct {
      GLuint color;
      GLuint position;
      GLuint constant;
      GLuint linear;
      GLuint exponential;

    } m_uniformPointLightLocations[MAX_POINT_LIGHTS];

    // SpotLights
    GLuint m_spotLightCount;
    GLuint m_uniformSpotLightCount;

    struct {
      GLuint color;
      GLuint position;
      GLuint direction;
      GLuint constant;
      GLuint linear;
      GLuint exponential;
      GLuint angle;

    } m_uniformSpotLightLocations[MAX_SPOT_LIGHTS];

    GLuint m_uniformTexture;
    GLuint m_uniformDirectionalLightTransform;
    GLuint m_uniformDirectionalShadowMap;

    //Material
    GLuint m_uniformSpecularIntensityLocation;
    GLuint m_uniformSpecularShininessLocation;

    //Omnidirectional shadowmap
    GLuint m_uniformOmniLightPos, m_uniformFarPlane;

    GLuint m_uniformLightMatrices[6];


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