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
#include <glm/gtx/transform.hpp>

using namespace glm;

//Load Shaders helper
#include <common/shader.hpp>
#include <common/vboindexer.hpp>

#include <playground/ImageLoader.hpp>
#include <playground/ControlSystem.hpp>
#include <playground/OBJLoader.hpp>

GLuint triangleVAO;
GLuint triangleVBO;

// Returns VAO's id
void CreateTriangle(GLuint* VAO, GLuint* VBO)
{
	// create VAO (vertex array object)
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);
		//Define a triangle vertices
		//An array of 3 vectors which represents 3 vertices
		// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
		// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
		
		static const GLfloat g_vertex_buffer_data[] = {
			-1.0f,-1.0f,0.0f, // triangle 1 : begin
			1.0f,-1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		};

		// generate one buffer
		glGenBuffers(1, VBO);
		//Bind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, *VBO);
		// Give vertices to OpenGL
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data,GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, inVertices.size() * sizeof(glm::vec3), &inVertices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(0);


		//Unbind vertex VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return 1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x multisampling
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Playground QW", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return 1;
	}

	int bufferWidth;
	int bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

	glfwMakeContextCurrent(window);
	glewExperimental=true; // Needed in core profile


	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	glViewport(0,0,bufferWidth, bufferHeight);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.4f, 0.0f, 0.2f, 0.0f);

	CreateTriangle(&triangleVAO, &triangleVBO);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );

	GLuint uniformOffset = glGetUniformLocation(programID, "posOffset");

	// // create MVP matrix
	// // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	// int width;
	// int height;
	// glfwGetWindowSize (window, &width, &height);
	// glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);  

	// // Or, for an ortho camera :
	// //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// // Camera matrix
	// glm::mat4 View = glm::lookAt(
	// 	glm::vec3(2,2,3), // Camera is at (4,3,3), in World Space
	// 	glm::vec3(0,0,0), // and looks at the origin
	// 	glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	// 	);

	// // Model matrix : an identity matrix (model will be at the origin)
	// glm::mat4 Model = glm::mat4(1.0f);
	// // Our ModelViewProjection : multiplication of our 3 matrices
	// glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

	// Get a handle for our "MVP" uniform
	// Only during the initialisation
	// GLuint ModelMatrixID = glGetUniformLocation(programID, "Model");
	// GLuint ViewMatrixID = glGetUniformLocation(programID, "View");
	// GLuint MVPMatrixID = glGetUniformLocation(programID, "MVP");
	// GLuint LightPosID = glGetUniformLocation(programID, "lightPosition_worldSpace");


	//Texture
	//GLuint Texture = loadDDS("uvtemplate.DDS");


	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);

	double lastTime = 0;

	//Performance monitoring
	int frameCount = 0;
	double frameTimer = 0;

	do{
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);
		lastTime = currentTime;

		// FPS counter
		frameCount++;
		if( currentTime - frameTimer > 1.0) // print once a second
		{
			fprintf(stdout, "%f ms/frame≤\n", 1000/double(frameCount));

			frameCount = 0;
			frameTimer += 1.0;
		}

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

		glUseProgram(programID);
		glUniform1f(uniformOffset, 0.5);

			glBindVertexArray(triangleVAO);

				// Draw the triangle !
				glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
				//glDrawElements(GL_TRIANGLES, indices.size() , GL_UNSIGNED_SHORT, 0); // 0 = offset

			glBindVertexArray(0);
		glUseProgram(0);
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

	// Needed?
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &triangleVBO);
	// glDeleteBuffers(1, &uvBuffer);
	// glDeleteBuffers(1, &colorBuffer);
	// glDeleteBuffers(1, &normalBuffer);
	// glDeleteBuffers(1, &indicesBuffer);
	glDeleteProgram(programID);
	// glDeleteTextures(1, &Texture);

	// Unbind & destroy
	glDeleteVertexArrays(1, &triangleVAO);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

