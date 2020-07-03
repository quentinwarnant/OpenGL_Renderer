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
	glClearColor(0.0f, 0.7f, 0.7f, 0.0f);



	// create VAO (vertex array object)
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
		//Define a triangle vertices
		//An array of 3 vectors which represents 3 vertices
		// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
		// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
		
		static const GLfloat g_vertex_buffer_data[] = {
			-1.0f,-1.0f,0.0f, // triangle 1 : begin
			1.0f,-1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
			// , // triangle 1 : end
			// 1.0f, 1.0f,-1.0f, // triangle 2 : begin
			// -1.0f,-1.0f,-1.0f,
			// -1.0f, 1.0f,-1.0f, // triangle 2 : end
			// 1.0f,-1.0f, 1.0f,
			// -1.0f,-1.0f,-1.0f,
			// 1.0f,-1.0f,-1.0f,
			// 1.0f, 1.0f,-1.0f,
			// 1.0f,-1.0f,-1.0f,
			// -1.0f,-1.0f,-1.0f,
			// -1.0f,-1.0f,-1.0f,
			// -1.0f, 1.0f, 1.0f,
			// -1.0f, 1.0f,-1.0f,
			// 1.0f,-1.0f, 1.0f,
			// -1.0f,-1.0f, 1.0f,
			// -1.0f,-1.0f,-1.0f,
			// -1.0f, 1.0f, 1.0f,
			// -1.0f,-1.0f, 1.0f,
			// 1.0f,-1.0f, 1.0f,
			// 1.0f, 1.0f, 1.0f,
			// 1.0f,-1.0f,-1.0f,
			// 1.0f, 1.0f,-1.0f,
			// 1.0f,-1.0f,-1.0f,
			// 1.0f, 1.0f, 1.0f,
			// 1.0f,-1.0f, 1.0f,
			// 1.0f, 1.0f, 1.0f,
			// 1.0f, 1.0f,-1.0f,
			// -1.0f, 1.0f,-1.0f,
			// 1.0f, 1.0f, 1.0f,
			// -1.0f, 1.0f,-1.0f,
			// -1.0f, 1.0f, 1.0f,
			// 1.0f, 1.0f, 1.0f,
			// -1.0f, 1.0f, 1.0f,
			// 1.0f,-1.0f, 1.0f
		};
		


		// // Read our .obj file
		// std::vector< glm::vec3 > inVertices;
		// std::vector< glm::vec2 > inUvs;
		// std::vector< glm::vec3 > inNormals; // Won't be used at the moment.
		// bool res = LoadOBJ("suzanne.obj", inVertices, inUvs, inNormals);

		// std::vector< glm::vec3 > vertices;
		// std::vector< glm::vec2 > uvs;
		// std::vector< glm::vec3 > normals; // Won't be used at the moment.
		// std::vector<unsigned short> indices;

		// //Fill indices vector
		// indexVBO_slow( inVertices, inUvs, inNormals, indices, vertices, uvs, normals);


		// One color for each vertex. They were generated randomly.
		static const GLfloat g_color_buffer_data[] = {
			0.583f,  0.771f,  0.014f,
			0.609f,  0.115f,  0.436f,
			0.327f,  0.483f,  0.844f,
			0.822f,  0.569f,  0.201f,
			0.435f,  0.602f,  0.223f,
			0.310f,  0.747f,  0.185f,
			0.597f,  0.770f,  0.761f,
			0.559f,  0.436f,  0.730f,
			0.359f,  0.583f,  0.152f,
			0.483f,  0.596f,  0.789f,
			0.559f,  0.861f,  0.639f,
			0.195f,  0.548f,  0.859f,
			0.014f,  0.184f,  0.576f,
			0.771f,  0.328f,  0.970f,
			0.406f,  0.615f,  0.116f,
			0.676f,  0.977f,  0.133f,
			0.971f,  0.572f,  0.833f,
			0.140f,  0.616f,  0.489f,
			0.997f,  0.513f,  0.064f,
			0.945f,  0.719f,  0.592f,
			0.543f,  0.021f,  0.978f,
			0.279f,  0.317f,  0.505f,
			0.167f,  0.620f,  0.077f,
			0.347f,  0.857f,  0.137f,
			0.055f,  0.953f,  0.042f,
			0.714f,  0.505f,  0.345f,
			0.783f,  0.290f,  0.734f,
			0.722f,  0.645f,  0.174f,
			0.302f,  0.455f,  0.848f,
			0.225f,  0.587f,  0.040f,
			0.517f,  0.713f,  0.338f,
			0.053f,  0.959f,  0.120f,
			0.393f,  0.621f,  0.362f,
			0.673f,  0.211f,  0.457f,
			0.820f,  0.883f,  0.371f,
			0.982f,  0.099f,  0.879f
		};

		GLuint vertexBuffer;
		// generate one buffer
		glGenBuffers(1, &vertexBuffer);
		//Bind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
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


		// //INDICES 
		// //Generate buffer for indices
		// GLuint indicesBuffer;
		// glGenBuffers(1, &indicesBuffer);
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
		// glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		// //VERT COLOR
		// GLuint colorBuffer;
		// glGenBuffers(1,&colorBuffer);
		// glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		// glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
		// glVertexAttribPointer(
		// 	1,                  // attribute 1. must match the layout in the shader.
		// 	3,                  // size
		// 	GL_FLOAT,           // type
		// 	GL_FALSE,           // normalized?
		// 	0,                  // stride
		// 	(void*)0            // array buffer offset
		// );
		// glEnableVertexAttribArray(1);
		// glBindBuffer(GL_ARRAY_BUFFER, 0);


		// UV data
		/*
		static const GLfloat g_uv_buffer_data[] =
		{
			0.000059f, 1.0f-0.000004f,
			0.000103f, 1.0f-0.336048f,
			0.335973f, 1.0f-0.335903f,
			1.000023f, 1.0f-0.000013f,
			0.667979f, 1.0f-0.335851f,
			0.999958f, 1.0f-0.336064f,
			0.667979f, 1.0f-0.335851f,
			0.336024f, 1.0f-0.671877f,
			0.667969f, 1.0f-0.671889f,
			1.000023f, 1.0f-0.000013f,
			0.668104f, 1.0f-0.000013f,
			0.667979f, 1.0f-0.335851f,
			0.000059f, 1.0f-0.000004f,
			0.335973f, 1.0f-0.335903f,
			0.336098f, 1.0f-0.000071f,
			0.667979f, 1.0f-0.335851f,
			0.335973f, 1.0f-0.335903f,
			0.336024f, 1.0f-0.671877f,
			1.000004f, 1.0f-0.671847f,
			0.999958f, 1.0f-0.336064f,
			0.667979f, 1.0f-0.335851f,
			0.668104f, 1.0f-0.000013f,
			0.335973f, 1.0f-0.335903f,
			0.667979f, 1.0f-0.335851f,
			0.335973f, 1.0f-0.335903f,
			0.668104f, 1.0f-0.000013f,
			0.336098f, 1.0f-0.000071f,
			0.000103f, 1.0f-0.336048f,
			0.000004f, 1.0f-0.671870f,
			0.336024f, 1.0f-0.671877f,
			0.000103f, 1.0f-0.336048f,
			0.336024f, 1.0f-0.671877f,
			0.335973f, 1.0f-0.335903f,
			0.667969f, 1.0f-0.671889f,
			1.000004f, 1.0f-0.671847f,
			0.667979f, 1.0f-0.335851f
		};
		*/

		// //UV
		// GLuint uvBuffer;
		// glGenBuffers(1, &uvBuffer);
		// glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		// //glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
		// glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
		// glVertexAttribPointer(
		// 	2,				// Attribute 2, same as shader
		// 	2,				// size of each entry (uv is vec2)
		// 	GL_FLOAT,		// type
		// 	GL_FALSE,		//normalised?
		// 	0,				//stride
		// 	(void*)0		// array buffer offset
		// );
		// glEnableVertexAttribArray(2);
		// glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// //Normal
		// GLuint normalBuffer;
		// glGenBuffers(1, &normalBuffer);
		// glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		// glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		// glVertexAttribPointer(
		// 	3,				// Attribute 3, same as shader
		// 	3,				// size of each entry (normal is vec3)
		// 	GL_FLOAT,		// type
		// 	GL_FALSE,		//normalised?
		// 	0,				//stride
		// 	(void*)0		// array buffer offset
		// );
		// glEnableVertexAttribArray(3);
		// glBindBuffer(GL_ARRAY_BUFFER, 0);
		
	glBindVertexArray(0);



	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );

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
	GLuint ModelMatrixID = glGetUniformLocation(programID, "Model");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "View");
	GLuint MVPMatrixID = glGetUniformLocation(programID, "MVP");
	GLuint LightPosID = glGetUniformLocation(programID, "lightPosition_worldSpace");


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
			glBindVertexArray(VertexArrayID);

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
	glDeleteBuffers(1, &vertexBuffer);
	// glDeleteBuffers(1, &uvBuffer);
	// glDeleteBuffers(1, &colorBuffer);
	// glDeleteBuffers(1, &normalBuffer);
	// glDeleteBuffers(1, &indicesBuffer);
	glDeleteProgram(programID);
	// glDeleteTextures(1, &Texture);

	// Unbind & destroy
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

