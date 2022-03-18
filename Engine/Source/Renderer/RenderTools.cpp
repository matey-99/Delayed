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

void RenderTools::Initialize()
{
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
