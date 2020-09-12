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

#include <assimp/Importer.hpp>
#include <playground/Model.hpp>

const float c_degToRad = 3.14159265f / 180.0f;
const GLsizei c_windowSizeW = 768;
const GLsizei c_windowSizeH = 480;

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
	Assimp::Importer importer = Assimp::Importer();
    importer.GetOrphanedScene();
    

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

void RenderScene( GLuint uniformModel, glm::mat4 modelMatrix,
                    GLuint uniformSpecIntensity, GLuint uniformSpecShininess,
                    Material* shinyMat, Material* dullMat, Texture* tex1, Texture* tex2,
                    std::vector<Mesh*> meshes, Model* externalModel,
                    float rotation)
{
    //Model
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.5f,2,-3));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5));
    modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0,1,0) );
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix) );

    //Mesh 1
    shinyMat->UseMaterial(uniformSpecIntensity, uniformSpecShininess);
    tex1->UseTexture();
    meshes[0]->RenderMesh();

    //Mesh 2
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(1.5f,3,-3));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix) );

    dullMat->UseMaterial(uniformSpecIntensity, uniformSpecShininess);
    tex2->UseTexture();
    meshes[1]->RenderMesh();

    // Floor plane
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0,-0.5,0));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix) );

    shinyMat->UseMaterial(uniformSpecIntensity, uniformSpecShininess);
    tex1->UseTexture();
    meshes[2]->RenderMesh();

    // external Model
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0,-0.5,0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2,0.2,0.2));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix) );
    shinyMat->UseMaterial(uniformSpecIntensity, uniformSpecShininess);
    externalModel->RenderModel();

}

void DirectionalShadowMapPass(Shader* directionalShadowShader,
                              Shader* shader, glm::mat4 modelMatrix, glm::mat4 projectionMatrix, glm::mat4 viewMatrix, Camera* camera,
                              LightDirectional* directionalLight, PointLight* pointLights, uint pointLightCount, SpotLight* spotLights, uint spotLightCount,
                              Material* shinyMat, Material* dullMat, Texture* tex1, Texture* tex2,
                              std::vector<Mesh*> meshes, Model* externalModel,
                              float rotation)
{

    directionalShadowShader->StartUseShader();

    ShadowMap* shadowMap =  directionalLight->GetShadowMap();
    //we need to make sure the viewport we're rendering in is the same size as the shadowmap
    glViewport(0,0,shadowMap->GetShadowWidth(), shadowMap->GetShadowHeight());

    shadowMap->Write(); //Start writing to it
    glClear(GL_DEPTH_BUFFER_BIT); //make sure the frame buffer is cleared before we start writing depth info to it


    GLuint uniformModel = directionalShadowShader->GetUniformModelLocation();
    directionalShadowShader->SetDirectionalLightTransform( directionalLight->CalculateLightTransform() );

    RenderScene( uniformModel, modelMatrix,
                 0, 0,
                shinyMat, dullMat, tex1, tex2,
                meshes, externalModel,
                rotation);

    directionalShadowShader->EndUseShader();

}

void MainRenderPass(Shader* shader, glm::mat4 modelMatrix, glm::mat4 projectionMatrix, glm::mat4 viewMatrix, Camera* camera,
                    LightDirectional* directionalLight, PointLight* pointLights, uint pointLightCount, SpotLight* spotLights, uint spotLightCount,
                    Material* shinyMat, Material* dullMat, Texture* tex1, Texture* tex2,
                    std::vector<Mesh*> meshes, Model* externalModel,
                    float rotation, GLfloat glWindowBufferWidth, GLfloat glWindowBufferHeight)
{
	// Re-bind the default framebuffer 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
	//we need to make sure the viewport we're rendering in is the same size as the window
    glViewport(0,0, glWindowBufferWidth, glWindowBufferHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


    shader->StartUseShader();

    //Model
    GLuint uniformModel = shader->GetUniformModelLocation();

    //Projection
    glUniformMatrix4fv(shader->GetUniformProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projectionMatrix) );

    //View
    viewMatrix = camera->CalculateViewMatrix();
    glUniformMatrix4fv(shader->GetUniformViewLocation(), 1, GL_FALSE, glm::value_ptr(viewMatrix));

    //Cam pos
    glUniform3fv(shader->GetUniformCameraWorldPosLocation(), 1, glm::value_ptr( camera->GetPos() ));

    //Setup Lights
    shader->SetDirectionalLight(directionalLight);
    shader->SetPointLights(pointLights, pointLightCount);
    shader->SetSpotLights(spotLights, spotLightCount);
    shader->SetDirectionalLightTransform( directionalLight->CalculateLightTransform());

    directionalLight->GetShadowMap()->Read(GL_TEXTURE1); //ShadowMap into Texture unit 1
    shader->SetTexture(0); // albedo texture set to texture unit 0
    shader->SetDirectionalShadowMap(1); // let the shader know that the shadowmap is in texture unit 1

    RenderScene( uniformModel, modelMatrix,
                shader->GetUniformSpecularIntensityLocation(), shader->GetUniformSpecularShininessLocation(),
                shinyMat, dullMat, tex1, tex2,
                meshes, externalModel,
                rotation);

    shader->EndUseShader();
}

int main(void)
{

	QWindow* window = new QWindow(c_windowSizeW, c_windowSizeH, "Playground QW");
	window->Init();

	GLfloat mousePosChangeX, mousePosChangeY;

	Camera* camera = new Camera(glm::vec3(0,0,0), glm::vec3(0,1,0), -90.0f, 0.0f, 1.0f, 3.0f);

	//Create two shapes
	std::vector<Mesh*> meshes;
	Mesh* pyramid1 = CreatePyramid();
	meshes.push_back(pyramid1);
	
	Mesh* pyramid2 = CreatePyramid();
	meshes.push_back(pyramid2);

	Mesh* floor = CreatePlane();
	meshes.push_back(floor);

	Model* externalModel = new Model();
	externalModel->LoadModel("../assets/Models/CobbleStones.obj");

    //Shadowmap shader
    Shader* directionalShadowShader = new Shader();
    directionalShadowShader->LoadShader("Shaders/directionalShadowMap_Vert.glsl", "Shaders/directionalShadowMap_Frag.glsl" );

	// Create and compile our GLSL program from the shaders
	Shader* shader = new Shader();
	shader->LoadShader("Shaders/SimpleShader_Vert.glsl", "Shaders/SimpleShader_Frag.glsl");

	Material* shinyMat = new Material(0.8f, 32);
	Material* dullMat = new Material(0.3f, 4);

	//Create textures
	Texture* tex1 = new Texture("../assets/brick.png");
    tex1->LoadTextureWithAlpha();

	Texture* tex2 = new Texture("../assets/dirt.png");
    tex2->LoadTextureWithAlpha();

	glm::vec3 ambientColor = glm::vec3(1.0f,1.0f,1.0f);
	GLfloat ambienIntensity = 0.2f;
	glm::vec3 lightDirection = glm::normalize(glm::vec3(0.0f, -1.0f, 0.5f) );
	LightDirectional* directionalLight = new LightDirectional(ambientColor, ambienIntensity,
                                                              glm::vec3(0.8f, 0.8f, 0.8f), 1.0f,
                                                              lightDirection, 1024, 1024);

	// Array of PointLights
	PointLight pointLights[] = {
		PointLight(glm::vec3(1.0f, 0.5f, -3.0f),
										glm::vec3(0,0,1), 0.3f,0.2f, 0.1f ),
		PointLight(glm::vec3(3.5f, 0.2f, -2.5f),
										glm::vec3(0,1,0), 0.3f,0.2f, 0.1f )
										};
	unsigned int pointLightCount = 2;

	//Array of SpotLights
	SpotLight spotLights[] = {
		SpotLight(glm::vec3(1.0f, .7f, -3.0f),
								glm::normalize( glm::vec3(0,-1,0) ),
								glm::vec3(1,0,1), 0.3f,0.2f, 0.1f,
								25.0f ),

		SpotLight(glm::vec3(-3.5f, 0.2f, -2.5f),
								glm::normalize( glm::vec3(0,-0.5,-1.0f ) ),
								glm::vec3(1,0,0), 0.3f,0.2f, 0.1f,
								40.0f )
	};

	unsigned int spotLightCount = 2;

	glm::mat4 projectionMatrix = glm::perspective(45.0f, (GLfloat) window->GetWindowBufferWidth() / (GLfloat) window->GetWindowBufferHeight(), 0.02f, 1000.0f );
	glm::mat4 viewMatrix = glm::mat4(1);
	glm::mat4 modelMatrix = glm::mat4(1);

	double lastTime = 0;
	//Performance monitoring
	int frameCount = 0;
	double frameTimer = 0;

	float rotationDegrees = 0;

	//Input
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
            ReloadShader(directionalShadowShader);
		}

		rotationDegrees += 40.0f * deltaTime;
		rotationDegrees = fmod( rotationDegrees, 360);
		float rotation = rotationDegrees * c_degToRad;

        DirectionalShadowMapPass( directionalShadowShader, shader, modelMatrix, projectionMatrix, viewMatrix, camera,
                                    directionalLight, pointLights, pointLightCount, spotLights, spotLightCount,
                                    shinyMat, dullMat, tex1, tex2,
                                    meshes, externalModel,
                                    rotation);

        MainRenderPass( shader, modelMatrix, projectionMatrix, viewMatrix, camera,
                          directionalLight, pointLights, pointLightCount, spotLights, spotLightCount,
                          shinyMat, dullMat, tex1, tex2,
                          meshes, externalModel,
                          rotation, window->GetWindowBufferWidth(), window->GetWindowBufferHeight());


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
	for (size_t i = 0; i < meshes.size(); i++)
	{
		delete(meshes[i]);
	}

	delete(camera);

	//cleanup window & context
	delete(window);
	
	
	return 0;
}

