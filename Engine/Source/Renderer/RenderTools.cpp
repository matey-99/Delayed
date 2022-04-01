#include "RenderTools.h"

#include <glad/glad.h>

RenderTools::RenderTools()
{
	Initialize();
}

void RenderTools::RenderQuad()
{
	glBindVertexArray(m_QuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void RenderTools::RenderBoundingBox(BoundingBox box)
{
	std::vector<glm::vec3> points = box.GetPoints();
	
	float boxVertices[] =
	{
		// Back Face
		points[0].x, points[0].y, points[0].z,
		points[6].x, points[6].y, points[6].z,
		points[4].x, points[4].y, points[4].z,
		points[6].x, points[6].y, points[6].z,
		points[0].x, points[0].y, points[0].z,
		points[2].x, points[2].y, points[2].z,

		// Front Face
		points[1].x, points[1].y, points[1].z,
		points[5].x, points[5].y, points[5].z,
		points[7].x, points[7].y, points[7].z,
		points[7].x, points[7].y, points[7].z,
		points[3].x, points[3].y, points[3].z,
		points[1].x, points[1].y, points[1].z,

		// Left Face
		points[3].x, points[3].y, points[3].z,
		points[2].x, points[2].y, points[2].z,
		points[0].x, points[0].y, points[0].z,
		points[0].x, points[0].y, points[0].z,
		points[1].x, points[1].y, points[1].z,
		points[3].x, points[3].y, points[3].z,

		// Right Face
		points[7].x, points[7].y, points[7].z,
		points[4].x, points[4].y, points[4].z,
		points[6].x, points[6].y, points[6].z,
		points[4].x, points[4].y, points[4].z,
		points[7].x, points[7].y, points[7].z,
		points[5].x, points[5].y, points[5].z,

		// Bottom Face
		points[0].x, points[0].y, points[0].z,
		points[4].x, points[4].y, points[4].z,
		points[5].x, points[5].y, points[5].z,
		points[5].x, points[5].y, points[5].z,
		points[1].x, points[1].y, points[1].z,
		points[0].x, points[0].y, points[0].z,

		// Top Face
		points[2].x, points[2].y, points[2].z,
		points[7].x, points[7].y, points[7].z,
		points[6].x, points[6].y, points[6].z,
		points[7].x, points[7].y, points[7].z,
		points[2].x, points[2].y, points[2].z,
		points[3].x, points[3].y, points[3].z
	};

	uint32_t vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void RenderTools::RenderSphere()
{
}

void RenderTools::Initialize()
{
	// Quad
	float quadVertices[] =
	{
		-1.0f,  1.0f,	0.0f, 1.0f,
		-1.0f, -1.0f,	0.0f, 0.0f,
		 1.0f, -1.0f,	1.0f, 0.0f,

		-1.0f,  1.0f,	0.0f, 1.0f,
		 1.0f, -1.0f,	1.0f, 0.0f,
		 1.0f,  1.0f,	1.0f, 1.0f,
	};

	uint32_t vbo;
	glGenVertexArrays(1, &m_QuadVAO);
	glGenBuffers(1, &vbo);
	glBindVertexArray(m_QuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
