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

}

Shader::~Shader()
{
    UnloadShader();
}

void Shader::LoadShader(const char* pathVertexShader, const char* pathFragmentShader)
{
	// Create the shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(pathVertexShader, std::ios::in);
	if(vertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << vertexShaderStream.rdbuf();
		vertexShaderCode = sstr.str();
		vertexShaderStream.close();
	}
    else
    {
		printf("Impossible to open %s. Are you in the right directory ?!\n", pathVertexShader);
		getchar();
	}

	// Read the Fragment Shader code from the file
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(pathFragmentShader, std::ios::in);
	if(fragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << fragmentShaderStream.rdbuf();
		fragmentShaderCode = sstr.str();
		fragmentShaderStream.close();
	}

	GLint result = GL_FALSE;
	int infoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", pathVertexShader);
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


	printf("Compiling shader : %s\n", pathFragmentShader);
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
    m_uniformModelLocation = glGetUniformLocation(programID, "Model");
	m_uniformViewLocation = glGetUniformLocation(programID, "View");
	m_uniformProjectionID = glGetUniformLocation(programID, "Projection");

	m_uniformAmbientColorLocation = glGetUniformLocation(programID, "directionalLight.color");
	m_uniformAmbientIntensityLocation = glGetUniformLocation(programID, "directionalLight.intensity");
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
    return m_uniformProjectionID;
}


GLuint Shader::GetUniformAmbientColorLocation()
{
    return m_uniformAmbientColorLocation;
}

GLuint Shader::GetUniformAmbientIntensityLocation()
{
    return m_uniformAmbientIntensityLocation;
}
