#include "Texture.h"

#include <glad/glad.h>

#include "Renderer/Renderer.h"

Texture::Texture(std::string path, uint16_t width, uint16_t height, uint16_t componentsCount, uint8_t* data)
	: m_Path(path), m_Width(width), m_Height(height), m_ComponentsCount(componentsCount)
{
	m_ID = 0;

	Load(data);
}

Texture::Texture(std::string path, uint16_t width, uint16_t height, uint16_t componentsCount, float* data)
	: m_Path(path), m_Width(width), m_Height(height), m_ComponentsCount(componentsCount)
{
	m_ID = 0;

	LoadHDR(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
}

Ref<Texture> Texture::Create(std::string path, uint16_t width, uint16_t height, uint16_t componentsCount, uint8_t* data)
{
	return CreateRef<Texture>(path, width, height, componentsCount, data);
}

Ref<Texture> Texture::CreateHDR(std::string path, uint16_t width, uint16_t height, uint16_t componentsCount, float* data)
{
	return CreateRef<Texture>(path, width, height, componentsCount, data);
}

void Texture::Load(uint8_t* data)
{
	if (m_ID)
	{
		glDeleteTextures(1, &m_ID);
	}

	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	if (m_ComponentsCount == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, m_Width, m_Height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	else if (m_ComponentsCount == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	else if (m_ComponentsCount == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Texture::LoadHDR(float* data)
{
	if (m_ID)
	{
		glDeleteTextures(1, &m_ID);
	}

	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::Bind(uint32_t index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
