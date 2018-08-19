#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//using namespace glm;

#include "meterial.h"

struct Vertex {
	// Position
	glm::vec3 Position;
	// Normal
	glm::vec3 Normal;
	// TexCoords
	glm::vec2 TexCoords;
};

class AABB {
public:
	AABB() {
		GLfloat xmax = -FLT_MAX, xmin = FLT_MAX, ymax = -FLT_MAX, ymin = FLT_MAX, zmax = -FLT_MAX, zmin = FLT_MAX;
	}
	float xmax, xmin, ymax, ymin, zmax, zmin;
};

class Mesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	Meterial *meterial;
	string meterialName;
	AABB aabb;
	/*  Functions  */
	// Constructor
	Mesh(vector<Vertex> _vertices, vector<GLuint> _indices, AABB _aabb, Meterial *_meterial = NULL , string _meterialName = "") 
		:meterial(_meterial), meterialName(_meterialName), aabb(_aabb)
	{
		this->vertices = _vertices;
		this->indices = _indices;
		// Now that we have all the required data, set the vertex buffers and its attribute pointers.
		this->setupMesh();
	}

	// Render the mesh,no bind shader
	void Draw()
	{
		// Draw mesh
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

private:
	/*  Render data  */
	GLuint VAO, VBO, EBO;

	/*  Functions    */
	// Initializes all the buffer objects/arrays
	void setupMesh()
	{
		// Create buffers/arrays
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);
		// Load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		// Set the vertex attribute pointers
		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
		// Vertex Texture Coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
};


