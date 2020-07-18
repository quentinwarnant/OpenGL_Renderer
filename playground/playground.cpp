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
//#include <playground/ControlSystem.hpp>
//#include <playground/OBJLoader.hpp>
#include <playground/Mesh.hpp>
#include <playground/Shader.hpp>
#include <playground/QWindow.hpp>
#include <playground/Camera.hpp>
#include <playground/Lighting/Light.hpp>

const float degToRad = 3.14159265f / 180.0f;

// Returns VAO's id
Mesh* CreateTriangle()
{
	//Define a triangle vertices
	//An array of 3 vectors which represents 3 vertices
	// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	
	GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f, 0.0f, /*UV*/ 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f, /*UV*/ 0.5f, 0.0f,
		1.0f,-1.0f, 0.0f,  /*UV*/ 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,  /*UV*/ 0.5f, 1.0f
	};

	unsigned int indices[] =
	{
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};
	
	Mesh* triangleMesh = new Mesh();
	unsigned int vertCount = sizeof(g_vertex_buffer_data) / sizeof(g_vertex_buffer_data[0]);
	unsigned int indicesCount = sizeof(indices) / sizeof(indices[0]);
	triangleMesh->CreateMesh(g_vertex_buffer_data, vertCount, indices, indicesCount);

	return triangleMesh;
}


int main( void )
{
	QWindow* window = new QWindow(768, 480, "Playground QW");	
	window->Init();

	GLfloat mousePosChangeX, mousePosChangeY;

	Camera* camera = new Camera(glm::vec3(0,0,0), glm::vec3(0,1,0), -90.0f, 0.0f, 1.0f, 3.0f);

	//Create two shapes
	std::vector<Mesh*> m_meshes;
	Mesh* triangle = CreateTriangle();
	m_meshes.push_back(triangle);
	
	Mesh* triangle2 = CreateTriangle();
	m_meshes.push_back(triangle2);

	// Create and compile our GLSL program from the shaders
	Shader* shader = new Shader();
	shader->LoadShader("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader"); 


	//Create textures
	Texture* tex1 = new Texture("../assets/brick.png");
	tex1->LoadTexture();

	Texture* tex2 = new Texture("../assets/dirt.png");
	tex2->LoadTexture();

	Light* ambientLight = new Light(glm::vec3(1.0f,1.0f,1.0f), 0.2f);

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
		printf("Mouse Diff x:%.6f y:%.6f \n", mousePosChangeX, mousePosChangeY);


		moveInput.x = window->IsKeyPressed(GLFW_KEY_W) ? 1 : (window->IsKeyPressed(GLFW_KEY_S) ? -1 : 0); 
		moveInput.y = window->IsKeyPressed(GLFW_KEY_D) ? 1 : (window->IsKeyPressed(GLFW_KEY_A) ? -1 : 0); 
		turnAxesInput.x = mousePosChangeX;
		turnAxesInput.y = mousePosChangeY;


		camera->Update(deltaTime, moveInput, turnAxesInput);


		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// Compute the MVP matrix from keyboard and mouse input
		// computeMatricesFromInputs(window, deltaTime);
		// glm::mat4 ProjectionMatrix = getProjectionMatrix();
		// glm::mat4 ViewMatrix = getViewMatrix();
		// glm::mat4 ModelMatrix = glm::mat4(1.0);
		// glm::mat4 mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		// glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		// glUniformMatrix4fv(MVPMatrixID, 1, GL_FALSE, &mvp[0][0]);

		
		// glm::vec3 lightPos = glm::vec3(4,4,4);
		// glUniform3f(LightPosID, lightPos.x, lightPos.y, lightPos.z);

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

		tex1->UseTexture();
		ambientLight->UseLight(shader->GetUniformAmbientColorLocation(), shader->GetUniformAmbientIntensityLocation());

		m_meshes[0]->RenderMesh();

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(1.5f,0,-3));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5));
		modelMatrix = glm::rotate(modelMatrix, -rotation, glm::vec3(0,1,0) );
		glUniformMatrix4fv(shader->GetUniformModelLocation(), 1, GL_FALSE, glm::value_ptr(modelMatrix) );
		
		tex2->UseTexture();
		m_meshes[1]->RenderMesh();

		shader->EndUseShader();
		
		// Swap buffers
		window->SwapBuffers();
		glfwPollEvents();

		window->PostUpdate();

	} // Check if the ESC key was pressed or the window was closed
	while( window->ShouldClose() == false );

	delete(ambientLight);

	delete(tex1);
	delete(tex2);

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

