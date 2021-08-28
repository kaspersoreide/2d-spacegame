#include "vertexarrays.h"

GLuint vertexArray2x3f(const vector<vec3>& vertices) {
	GLuint VBO, VAO;
	glCreateBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//writing all the vertices into the buffer
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(vec3) * vertices.size(),
		&vertices[0][0],
		GL_STATIC_DRAW
	);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		6 * sizeof(float),
		(void*)0
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		6 * sizeof(float),
		(void*)(3 * sizeof(float))
	);

	return VAO;
}

GLuint vertexArray1x3f(const vector<vec3>& vertices) {
	GLuint VBO, VAO;
	glCreateBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//writing all the vertices into the buffer
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(vec3) * vertices.size(),
		&vertices[0][0],
		GL_STATIC_DRAW
	);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(float),
		(void*)0
	);

	return VAO;
}

GLuint vertexArray1x2f(vector<vec2>& vertices){
	GLuint VBO, VAO;
	glCreateBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//writing all the vertices into the buffer
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(vec2) * vertices.size(),
		&vertices[0][0],
		GL_STATIC_DRAW
	);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		2,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(float),
		(void*)0
	);

	return VAO;
}
