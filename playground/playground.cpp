// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/transform.hpp>

using namespace glm;


//Load Shaders helper
//#include <common/shader.hpp>
//#include <common/vboindexer.hpp>

//Image loading library STB_IMAGE - SOIL could be an alternative
#define STB_IMAGE_IMPLEMENTATION

#include <playground/Texture.hpp>
#include <playground/Settings.hpp>
#include <playground/Mesh.hpp>
#include <playground/Shader.hpp>
#include <playground/Material.hpp>
#include <playground/QWindow.hpp>
#include <playground/Camera.hpp>
#include <playground/Lighting/LightDirectional.hpp>
#include <playground/Lighting/PointLight.hpp>
#include <playground/Lighting/SpotLight.hpp>


const float degToRad = 3.14159265f / 180.0f;

void CalculateAverageNormals(unsigned int* indices, unsigned int indicesCount, GLfloat* vertices, unsigned int vertCount,
								unsigned int vertexDataLength , unsigned int dataNormalOffset)
{

	for(size_t i=0; i < indicesCount; i+=3) //iterate through each pyramid1
	{
		unsigned int in0 = indices[i] * vertexDataLength;
		unsigned int in1 = indices[i+1] * vertexDataLength;
		unsigned int in2 = indices[i+2] * vertexDataLength;
		//Get vector 0-1 & 0-2
		glm::vec3 vec01( vertices[in1] - vertices[in0], vertices[in1+1] - vertices[in0+1], vertices[in1+2] - vertices[in0+2]);
		glm::vec3 vec02( vertices[in2] - vertices[in0], vertices[in2+1] - vertices[in0+1], vertices[in2+2] - vertices[in0+2]);

		glm::vec3 normal = glm::normalize( glm::cross(vec01,vec02) );
		in0 += dataNormalOffset;
		in1 += dataNormalOffset;
		in2 += dataNormalOffset;
		
		//adding normal value to each vertices in pyramid1 
		vertices[in0] += normal.x;
		vertices[in0+1] += normal.y;
		vertices[in0+2] += normal.z;
		
		vertices[in1] += normal.x;
		vertices[in1+1] += normal.y;
		vertices[in1+2] += normal.z;

		vertices[in2] += normal.x;
		vertices[in2+1] += normal.y;
		vertices[in2+2] += normal.z;
	}

	//each vertex was iterated over multiple times, so we want to normalize the normals now
	for(size_t i=0; i < vertCount / vertexDataLength; i++)
	{
		unsigned int normalOffset = (i * vertexDataLength) + dataNormalOffset;
		glm::vec3 normal(vertices[normalOffset], vertices[normalOffset+1], vertices[normalOffset+2]);
		normal = glm::normalize(normal);

		vertices[normalOffset] = normal.x;
		vertices[normalOffset+1] = normal.y;
		vertices[normalOffset+2] = normal.z;
	}
}

// Returns VAO's id
Mesh* CreatePyramid()
{
	//Define a pyramid1 vertices
	//An array of 3 vectors which represents 3 vertices
	// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a pyramid1.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	
	GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f, -0.6f, /*UV*/ 0.0f, 0.0f, /*Normal*/ 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f, /*UV*/ 0.5f, 0.0f, /*Normal*/ 0.0f, 0.0f, 0.0f,
		1.0f,-1.0f, -0.6f,  /*UV*/ 1.0f, 0.0f, /*Normal*/ 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,  /*UV*/ 0.5f, 1.0f, /*Normal*/ 0.0f, 0.0f, 0.0f
	};

	unsigned int indices[] =
	{
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	
	Mesh* mesh = new Mesh();
	unsigned int dataPerVertex = 8;
	unsigned int vertCount = sizeof(g_vertex_buffer_data) / sizeof(g_vertex_buffer_data[0]);
	unsigned int indicesCount = sizeof(indices) / sizeof(indices[0]);
	
	CalculateAverageNormals(indices, indicesCount, g_vertex_buffer_data, vertCount,dataPerVertex, dataPerVertex - 3 );
	
	mesh->CreateMesh(g_vertex_buffer_data, vertCount, indices, indicesCount);

	return mesh;
}

Mesh* CreatePlane()
{
		GLfloat g_vertex_buffer_data[] = {
		-10.0f,0.0f, -10.0f, /*UV*/ 0.0f, 0.0f, /*Normal*/ 0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f, /*UV*/ 10.0f, 0.0f, /*Normal*/ 0.0f, -1.0f, 0.0f,
		-10.0f,0.0f, 10.0f,  /*UV*/ 0.0f, 10.0f, /*Normal*/ 0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,  /*UV*/ 10.0f, 10.0f, /*Normal*/ 0.0f, -1.0f, 0.0f
	};

	unsigned int indices[] =
	{
		0, 2, 1,
		1, 2, 3
	};

	Mesh* mesh = new Mesh();
	unsigned int vertCount = sizeof(g_vertex_buffer_data) / sizeof(g_vertex_buffer_data[0]);
	unsigned int indicesCount = sizeof(indices) / sizeof(indices[0]);

	mesh->CreateMesh(g_vertex_buffer_data, vertCount, indices, indicesCount);

	return mesh;
}

void ReloadShader(Shader* shaderToReload)
{
	shaderToReload->ReloadSources();
}

int main( void )
{
	QWindow* window = new QWindow(768, 480, "Playground QW");	
	window->Init();

	GLfloat mousePosChangeX, mousePosChangeY;

	Camera* camera = new Camera(glm::vec3(0,0,0), glm::vec3(0,1,0), -90.0f, 0.0f, 1.0f, 3.0f);

	//Create two shapes
	std::vector<Mesh*> m_meshes;
	Mesh* pyramid1 = CreatePyramid();
	m_meshes.push_back(pyramid1);
	
	Mesh* pyramid2 = CreatePyramid();
	m_meshes.push_back(pyramid2);

	Mesh* floor = CreatePlane();
	m_meshes.push_back(floor);

	// Create and compile our GLSL program from the shaders
	Shader* shader = new Shader();
	shader->LoadShader("SimpleVertexShader.shader", "SimpleFragmentShader.shader");

	Material* shinyMat = new Material(0.8f, 32);
	Material* dullMat = new Material(0.3f, 4);

	//Create textures
	Texture* tex1 = new Texture("../assets/brick.png");
    tex1->LoadTextureNoAlpha();

	Texture* tex2 = new Texture("../assets/dirt.png");
    tex2->LoadTextureNoAlpha();

	glm::vec3 ambientColor = glm::vec3(1.0f,1.0f,1.0f);
	GLfloat ambienIntensity = 0.2f;
	glm::vec3 lightDirection = glm::normalize(glm::vec3(0.5f, -1.0f, 0.5f) ); 
	LightDirectional* directionalLight = new LightDirectional(	ambientColor, ambienIntensity,
																glm::vec3(0.8f, 0.8f, 0.8f), 1.0f,
																lightDirection);

	// Array of PointLights
	PointLight pointLights[MAX_POINT_LIGHTS];
	pointLights[0] = PointLight(glm::vec3(1.0f, 0.5f, -3.0f),
										glm::vec3(0,0,1), 0.3f,0.2f, 0.1f );
	pointLights[1] = PointLight(glm::vec3(3.5f, 0.2f, -2.5f),
										glm::vec3(0,1,0), 0.3f,0.2f, 0.1f );
	unsigned int pointLightCount = 2;

	//Array of SpotLights
	SpotLight spotLights[MAX_SPOT_LIGHTS];
	spotLights[0] = SpotLight(glm::vec3(1.0f, .7f, -3.0f),
								glm::normalize( glm::vec3(0,-1,0) ),
								glm::vec3(1,0,1), 0.3f,0.2f, 0.1f,
								25.0f );

	spotLights[1] = SpotLight(glm::vec3(-3.5f, 0.2f, -2.5f),
								glm::normalize( glm::vec3(0,-0.5,-1.0f ) ),
								glm::vec3(1,0,0), 0.3f,0.2f, 0.1f,
								40.0f );

	unsigned int spotLightCount = 2;


	glm::mat4 projectionMatrix = glm::perspective(45.0f, (GLfloat) window->GetWindowBufferWidth() / (GLfloat) window->GetWindowBufferHeight(), 0.02f, 1000.0f );

	glm::mat4 viewMatrix = mat4(1);
	glm::mat4 modelMatrix = mat4(1);

	double lastTime = 0;

	//Performance monitoring
	int frameCount = 0;
	double frameTimer = 0;

	float rotationDegrees = 0;

	glm::vec2 moveInput = glm::vec2(0);
	glm::vec2 turnAxesInput = glm::vec2(0);
	

	do{
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);
		lastTime = currentTime;

		// FPS counter
		frameCount++;
		if( currentTime - frameTimer > 1.0) // print once a second
		{
			//fprintf(stdout, "%f ms/frame≤\n", 1000/double(frameCount));

			frameCount = 0;
			frameTimer += 1.0;
		}

		//Get mouse pos change
		window->PreUpdate();
		window->Update();

		window->GetMouseChange(mousePosChangeX, mousePosChangeY, true);
		//printf("Mouse Diff x:%.6f y:%.6f \n", mousePosChangeX, mousePosChangeY);


		moveInput.x = window->IsKeyPressed(GLFW_KEY_W) ? 1 : (window->IsKeyPressed(GLFW_KEY_S) ? -1 : 0); 
		moveInput.y = window->IsKeyPressed(GLFW_KEY_D) ? 1 : (window->IsKeyPressed(GLFW_KEY_A) ? -1 : 0); 
		turnAxesInput.x = mousePosChangeX;
		turnAxesInput.y = mousePosChangeY;


		camera->Update(deltaTime, moveInput, turnAxesInput);

		if( window->IsKeyPressed(GLFW_KEY_SPACE))
		{
			ReloadShader(shader);
		}


		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		
		rotationDegrees += 40.0f * deltaTime;
		rotationDegrees = fmod( rotationDegrees, 360);

		float rotation = rotationDegrees * degToRad;

		shader->StartUseShader();
		glUniformMatrix4fv(shader->GetUniformProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projectionMatrix) );

		//Model
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.5f,0,-3));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5));
		modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0,1,0) );
		glUniformMatrix4fv(shader->GetUniformModelLocation(), 1, GL_FALSE, glm::value_ptr(modelMatrix) );


		//View
		viewMatrix = camera->CalculateViewMatrix();
		glUniformMatrix4fv(shader->GetUniformViewLocation(), 1, GL_FALSE, glm::value_ptr(viewMatrix));

		//Cam pos
		glUniform3fv(shader->GetUniformCameraWorldPosLocation(), 1, glm::value_ptr( camera->GetPos() ));

		//Setup Lights
		shader->SetDirectionalLight(directionalLight);
		shader->SetPointLights(pointLights, pointLightCount);
		shader->SetSpotLights(spotLights, spotLightCount);


		//Mesh 1
		shinyMat->UseMaterial(shader->GetUniformSpecularIntensityLocation(), shader->GetUniformSpecularShininessLocation());
		tex1->UseTexture();
		m_meshes[0]->RenderMesh();

		//Mesh 2
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(1.5f,0,-3));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5));
		glUniformMatrix4fv(shader->GetUniformModelLocation(), 1, GL_FALSE, glm::value_ptr(modelMatrix) );
		
		dullMat->UseMaterial(shader->GetUniformSpecularIntensityLocation(), shader->GetUniformSpecularShininessLocation());
		tex2->UseTexture();
		m_meshes[1]->RenderMesh();


        // Floor plane
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0,-0.5,0));
		glUniformMatrix4fv(shader->GetUniformModelLocation(), 1, GL_FALSE, glm::value_ptr(modelMatrix) );
		
		shinyMat->UseMaterial(shader->GetUniformSpecularIntensityLocation(), shader->GetUniformSpecularShininessLocation());
		tex1->UseTexture();
		m_meshes[2]->RenderMesh();


		shader->EndUseShader();
		
		// Swap buffers
		window->SwapBuffers();
		glfwPollEvents();

		window->PostUpdate();

	} // Check if the ESC key was pressed or the window was closed
	while( window->ShouldClose() == false );

	//delete(pointLights);
	delete(directionalLight);
	
	delete(tex1);
	delete(tex2);

	delete(shinyMat);
	delete(dullMat);

	// Cleanup shader
	delete(shader);

	// Cleanup meshes
	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		delete(m_meshes[i]);
	}

	delete(camera);

	//cleanup window & context
	delete(window);
	
	
	return 0;
}

