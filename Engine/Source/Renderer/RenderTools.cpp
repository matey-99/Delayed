#include "RenderTools.h"

#include <glad/glad.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/noise.hpp>

#include "Math/Math.h"

#define SPHERE_COLLIDER_SEGMENTS 32

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

void RenderTools::RenderSkybox()
{
	glBindVertexArray(m_SkyboxVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
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

void RenderTools::RenderBoundingSphere(BoundingSphere sphere)
{
	// First circle
	glm::vec3 center = sphere.Center;
	float radius = sphere.Radius;
	float circleVertices[SPHERE_COLLIDER_SEGMENTS * 3];
	float angle = 0.0f;
	for (int i = 0; i < (SPHERE_COLLIDER_SEGMENTS * 3);)
	{
		float theta = 2.0f * glm::pi<float>() * angle / (float)(SPHERE_COLLIDER_SEGMENTS);

		float x = radius * glm::cos(theta);
		float y = radius * glm::sin(theta);

		circleVertices[i] = x + center.x;
		circleVertices[i + 1] = y + center.y;
		circleVertices[i + 2] = center.z;

		i += 3;
		angle++;
	}

	uint32_t vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glDrawArrays(GL_LINES, 0, SPHERE_COLLIDER_SEGMENTS);
	glBindVertexArray(0);

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	// Second circle
	angle = 0.0f;
	for (int i = 0; i < (SPHERE_COLLIDER_SEGMENTS * 3);)
	{
		float theta = 2.0f * glm::pi<float>() * angle / (float)(SPHERE_COLLIDER_SEGMENTS);

		float x = radius * glm::cos(theta);
		float z = radius * glm::sin(theta);

		circleVertices[i] = x + center.x;
		circleVertices[i + 1] = center.y;
		circleVertices[i + 2] = z + center.z;

		i += 3;
		angle++;
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glDrawArrays(GL_LINES, 0, SPHERE_COLLIDER_SEGMENTS);
	glBindVertexArray(0);

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	// Third circle
	angle = 0.0f;
	for (int i = 0; i < (SPHERE_COLLIDER_SEGMENTS * 3);)
	{
		float theta = 2.0f * glm::pi<float>() * angle / (float)(SPHERE_COLLIDER_SEGMENTS);

		float y = radius * glm::cos(theta);
		float z = radius * glm::sin(theta);

		circleVertices[i] = center.x;
		circleVertices[i + 1] = y + center.y;
		circleVertices[i + 2] = z + center.z;

		i += 3;
		angle++;
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glDrawArrays(GL_LINES, 0, SPHERE_COLLIDER_SEGMENTS);
	glBindVertexArray(0);

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
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

	uint32_t quadVBO;
	glGenVertexArrays(1, &m_QuadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(m_QuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Skybox
	float skyboxVertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	uint32_t skyboxVBO;
	glGenVertexArrays(1, &m_SkyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(m_SkyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

uint32_t RenderTools::GenerateSimpleNoiseTexture(uint64_t seed, uint32_t width, uint32_t height, uint32_t depth, uint32_t internalFormat)
{
	srand(seed);

	uint8_t* data = new uint8_t[width * height * depth * 4];
	uint8_t* ptr = data;

	for (int z = 0; z < depth; ++z)
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				*ptr++ = rand() & 0xff;
				*ptr++ = rand() & 0xff;
				*ptr++ = rand() & 0xff;
				*ptr++ = rand() & 0xff;
			}
		}
	}

	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_3D, texture);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, width, height, depth, 0, GL_RGBA, GL_BYTE, data);

	delete[] data;
	return texture;
}

uint32_t RenderTools::GenerateWorleyNoiseTexture(uint64_t seed, uint32_t width, uint32_t height, uint32_t depth, uint32_t internalFormat)
{
	srand(seed);

	uint8_t* data = new uint8_t[width * height * depth * 4];
	uint8_t* ptr = data;

	float cellSize = 1.0f / width;

	for (int z = 0; z < depth; ++z)
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				glm::vec3 offset = glm::vec3(rand() & 0xff);

				*ptr++ = rand() & 0xff;
				*ptr++ = rand() & 0xff;
				*ptr++ = rand() & 0xff;
				*ptr++ = rand() & 0xff;
			}
		}
	}

	return uint32_t();
}

uint32_t RenderTools::GeneratePerlinNoiseTexture(uint64_t seed, uint32_t width, uint32_t height, uint32_t depth, uint32_t internalFormat)
{
	srand(seed);

	float* data = new float[width * height * depth * 4];
	float* ptr = data;

	for (int z = 0; z < depth; ++z)
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				float perlin = glm::perlin(glm::vec3(x + 0.8f, y + 0.8f, z + 0.8f));
				*ptr++ = perlin;
				*ptr++ = perlin;
				*ptr++ = perlin;
				*ptr++ = perlin;
			}
		}
	}

	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_3D, texture);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, width, height, depth, 0, GL_RGBA, GL_FLOAT, data);

	delete[] data;
	return texture;
}
