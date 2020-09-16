#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <stdlib.h>

#include <GL/glew.h>

#include "Shader.hpp"

Shader::Shader()
{
	m_pointLightCount = 0;
}

Shader::~Shader()
{
    UnloadShader();
}

void Shader::LoadShader(const char* pathVertexShader, const char* pathFragmentShader)
{
	m_pathVertexShader = pathVertexShader;
	m_pathFragmentShader = pathFragmentShader;

	// Create the shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(m_pathVertexShader, std::ios::in);
	if(vertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << vertexShaderStream.rdbuf();
		vertexShaderCode = sstr.str();
		vertexShaderStream.close();
	}
    else
    {
		printf("Impossible to open %s. Are you in the right directory ?!\n", m_pathVertexShader);
		getchar();
	}

	// Read the Fragment Shader code from the file
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(m_pathFragmentShader, std::ios::in);
	if(fragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << fragmentShaderStream.rdbuf();
		fragmentShaderCode = sstr.str();
		fragmentShaderStream.close();
	}

	GLint result = GL_FALSE;
	int infoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", m_pathVertexShader);
	char const * vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource( vertexShaderID, 1, &vertexSourcePointer , NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader compilation
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	if ( !result ){
		glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> vertexShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
		
		throw new ShaderException("Compilation of vertex shader failed");
	}


	printf("Compiling shader : %s\n", m_pathFragmentShader);
	char const * fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer , NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader Compilation
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	if ( !result ){
		glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> fragmentShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		printf("%s\n", &fragmentShaderErrorMessage[0]);
		
		throw new ShaderException("Compilation of fragment shader failed");
	}

	// Link the program
	printf("Linking shader program\n");
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Check the program Linking result
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	if ( !result ){
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> programErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);

		throw new ShaderException("Linking of shader program failed");
	}


	// Error "no VAO bound" - should this only get checked when using it?
	// glValidateProgram(programID);
	// glGetProgramiv(programID, GL_VALIDATE_STATUS, &result);
	// if ( !result ){
	// 	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	// 	std::vector<char> programErrorMessage(infoLogLength+1);
	// 	glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
	// 	printf("%s\n", &programErrorMessage[0]);

	// 	throw new ShaderException("Validation of shader program failed");
	// }

	
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	m_shaderProgramLocation = programID;


    //Find uniforms
    m_uniformModelLocation 		= glGetUniformLocation(programID, "Model");
	m_uniformViewLocation 		= glGetUniformLocation(programID, "View");
	m_uniformProjectionLocation = glGetUniformLocation(programID, "Projection");

	m_uniformCameraWorldPosLocation = glGetUniformLocation(programID, "cameraWorldPos");
	
	m_uniformDirectionaLightLocations.ambientColor 		= glGetUniformLocation(programID, "dirLight.ambientColor");
	m_uniformDirectionaLightLocations.ambientIntensity 	= glGetUniformLocation(programID, "dirLight.ambientIntensity");

	m_uniformDirectionaLightLocations.directionalColor	= glGetUniformLocation(programID, "dirLight.directionalColor");
	m_uniformDirectionaLightLocations.directionalColorIntensity = glGetUniformLocation(programID, "dirLight.directionalIntensity");
	m_uniformDirectionaLightLocations.direction 		= glGetUniformLocation(programID, "dirLight.direction");


	//Point Lights
	m_uniformPointLightCount = glGetUniformLocation(programID,"pointLightCount");
	for(size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		//make up the location string
		char buff[100] = {'\0'};

		snprintf(buff, sizeof(buff), "pointLights[%zu].position", i);
		m_uniformPointLightLocations[i].position = glGetUniformLocation(programID,buff);

		snprintf(buff, sizeof(buff), "pointLights[%zu].color", i);
		m_uniformPointLightLocations[i].color = glGetUniformLocation(programID,buff);

		snprintf(buff, sizeof(buff), "pointLights[%zu].constant", i);
		m_uniformPointLightLocations[i].constant = glGetUniformLocation(programID,buff);

		snprintf(buff, sizeof(buff), "pointLights[%zu].linear", i);
		m_uniformPointLightLocations[i].linear = glGetUniformLocation(programID,buff);

		snprintf(buff, sizeof(buff), "pointLights[%zu].exponential", i);
		m_uniformPointLightLocations[i].exponential = glGetUniformLocation(programID,buff);

	}

	//Spot Lights (spot lights contains a point light info / derives from it)
	m_uniformSpotLightCount = glGetUniformLocation(programID,"spotLightCount");
	for(size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		//make up the location string
		char buff[100] = {'\0'};

		snprintf(buff, sizeof(buff), "spotLights[%zu].point.position", i);
		m_uniformSpotLightLocations[i].position = glGetUniformLocation(programID,buff);

		snprintf(buff, sizeof(buff), "spotLights[%zu].point.color", i);
		m_uniformSpotLightLocations[i].color = glGetUniformLocation(programID,buff);

		snprintf(buff, sizeof(buff), "spotLights[%zu].point.constant", i);
		m_uniformSpotLightLocations[i].constant = glGetUniformLocation(programID,buff);

		snprintf(buff, sizeof(buff), "spotLights[%zu].point.linear", i);
		m_uniformSpotLightLocations[i].linear = glGetUniformLocation(programID,buff);

		snprintf(buff, sizeof(buff), "spotLights[%zu].point.exponential", i);
		m_uniformSpotLightLocations[i].exponential = glGetUniformLocation(programID,buff);

		snprintf(buff, sizeof(buff), "spotLights[%zu].direction", i);
		m_uniformSpotLightLocations[i].direction = glGetUniformLocation(programID,buff);

		snprintf(buff, sizeof(buff), "spotLights[%zu].angle", i);
		m_uniformSpotLightLocations[i].angle = glGetUniformLocation(programID,buff);
	}
    m_uniformTexture = glGetUniformLocation(programID, "mainTexSampler");
    m_uniformDirectionalLightTransform = glGetUniformLocation(programID, "directionalLightSpaceTransform");
    m_uniformDirectionalShadowMap = glGetUniformLocation(programID, "directionalShadowMap");

	m_uniformSpecularIntensityLocation = glGetUniformLocation(programID, "material.specularIntensity");
	m_uniformSpecularShininessLocation = glGetUniformLocation(programID, "material.shininess");
}

void Shader::ReloadSources()
{
	UnloadShader();
	LoadShader(m_pathVertexShader, m_pathFragmentShader);
}

void Shader::SetDirectionalLight(LightDirectional* light)
{
	light->UseLight(m_uniformDirectionaLightLocations.ambientColor, m_uniformDirectionaLightLocations.ambientIntensity,
							m_uniformDirectionaLightLocations.directionalColor, m_uniformDirectionaLightLocations.directionalColorIntensity, m_uniformDirectionaLightLocations.direction);
}

void Shader::SetPointLights(PointLight* lights, GLuint pointLightCount)
{
	//Clamp
	if( pointLightCount > MAX_POINT_LIGHTS){ pointLightCount = MAX_POINT_LIGHTS;}

	m_pointLightCount = pointLightCount;
	glUniform1i(m_uniformPointLightCount, m_pointLightCount );

	for (size_t i = 0; i < m_pointLightCount; i++)
	{
		lights[i].UseLight( m_uniformPointLightLocations[i].position, m_uniformPointLightLocations[i].color,
								m_uniformPointLightLocations[i].constant, m_uniformPointLightLocations[i].linear, m_uniformPointLightLocations[i].exponential);
	}
}

void Shader::SetSpotLights(SpotLight* lights, GLuint spotLightCount)
{
	//Clamp
	if( spotLightCount > MAX_SPOT_LIGHTS){ spotLightCount = MAX_POINT_LIGHTS;}

	m_spotLightCount = spotLightCount;
	glUniform1i(m_uniformSpotLightCount, m_spotLightCount );

	for (size_t i = 0; i < m_spotLightCount; i++)
	{
		lights[i].UseLight(m_uniformSpotLightLocations[i].position, m_uniformSpotLightLocations[i].direction, m_uniformSpotLightLocations[i].color,
								m_uniformSpotLightLocations[i].constant, m_uniformSpotLightLocations[i].linear, m_uniformSpotLightLocations[i].exponential,
								m_uniformSpotLightLocations[i].angle);
	}
}


void Shader::UnloadShader()
{
	glDeleteProgram(m_shaderProgramLocation);
}

void Shader::StartUseShader()
{
    glUseProgram(m_shaderProgramLocation);
}

void Shader::EndUseShader()
{
    glUseProgram(0);
}

GLuint Shader::GetUniformModelLocation()
{
    return m_uniformModelLocation;
}

GLuint Shader::GetUniformViewLocation()
{
	return m_uniformViewLocation;
}
    
GLuint Shader::GetUniformProjectionLocation()
{
    return m_uniformProjectionLocation;
}

GLuint Shader::GetUniformCameraWorldPosLocation()
{
	return m_uniformCameraWorldPosLocation;
}


//Lighting
GLuint Shader::GetUniformAmbientColorLocation()
{
    return m_uniformDirectionaLightLocations.ambientColor;
}

GLuint Shader::GetUniformAmbientIntensityLocation()
{
    return m_uniformDirectionaLightLocations.ambientIntensity;
}

GLuint Shader::GetUniformDirectionalLightColorLocation()
{
    return m_uniformDirectionaLightLocations.directionalColor;
}

GLuint Shader::GetUniformDirectionalLightIntensityLocation()
{
	return m_uniformDirectionaLightLocations.directionalColorIntensity;
}

GLuint Shader::GetUniformDirectionalLightDirectionLocation()
{
	return m_uniformDirectionaLightLocations.direction;
}

//Material
GLuint Shader::GetUniformSpecularIntensityLocation()
{
	return m_uniformSpecularIntensityLocation;
}

GLuint Shader::GetUniformSpecularShininessLocation()
{
	return m_uniformSpecularShininessLocation;
}

void Shader::SetTexture(GLuint textureUnit)
{
    glUniform1i(m_uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
    glUniform1i(m_uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4 *lightTransform)
{
    glUniformMatrix4fv(m_uniformDirectionalLightTransform,1, GL_FALSE, glm::value_ptr(*lightTransform));
}

