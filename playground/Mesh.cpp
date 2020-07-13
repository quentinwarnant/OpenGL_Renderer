#include "Mesh.hpp"

Mesh::Mesh()
{
    VAO = VBO = IBO = 0; // init to 0
    indexCount = 0;
}

Mesh::~Mesh()
{
    ClearMesh();
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int vertCount, unsigned int* indices, unsigned int indicesCount)
{
    indexCount = indicesCount;

    // create VAO (vertex array object)
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
		// generate one buffer
		glGenBuffers(1, &VBO);
		//Bind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Give vertices to OpenGL
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertCount, vertices, GL_STATIC_DRAW);

        //Position
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			sizeof(vertices[0]) * 5,// stride , format is 3 vertices & 2 uv value
			(void*)0            // array buffer offset
		);
        // Enabling these vertex attributes; is fine from the create mesh until we have multiple sub-meshes to render, in which case we may have different attributes 
		glEnableVertexAttribArray(0); 

        //UV
        glVertexAttribPointer(
			1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			sizeof(vertices[0]) * 5,// stride , format is 3 vertices & 2 uv value
			(void*)(sizeof(vertices[0]) * 3)// array buffer offset
		);
		glEnableVertexAttribArray(1);


		//Unbind vertex VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// let's setup the index buffer now
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indicesCount, indices, GL_STATIC_DRAW); 

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0); // TUTORIAL SAYS TO UNBIND AFTER VAO - but... doesn't make sense? s 

        //Now UV attribute




	glBindVertexArray(0);
}

void Mesh::RenderMesh()
{
    glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glDrawElements(GL_TRIANGLES, indexCount  , GL_UNSIGNED_INT, 0); // 0 = offset
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
    if( IBO != 0)
    {
        glDeleteBuffers(1, &IBO);
        IBO = 0;
    }
    if( VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if( VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
}