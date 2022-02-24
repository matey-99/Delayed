#pragma once

#include <glm/glm.hpp>

#include <glad/glad.h>

struct FramebufferTextureConfig
{
	int Attachment = GL_COLOR_ATTACHMENT0;
	int Target = GL_TEXTURE_2D;

	int InternalFormat = GL_RGBA32F;
	int Format = GL_RGBA;

	int MinFilter = GL_LINEAR;
	int MagFilter = GL_LINEAR;

	int WrapS = GL_NONE;
	int WrapT = GL_NONE;
	int WrapR = GL_NONE;

	int Type = GL_UNSIGNED_BYTE;

	bool Mipmap = false;
	bool Border = false;
};

struct FramebufferRenderbufferConfig
{
	int Attachment = GL_DEPTH_STENCIL_ATTACHMENT;
	int InternalFormat = GL_DEPTH_STENCIL;
};

struct FramebufferConfig
{
	uint32_t Width = 1024;
	uint32_t Height = 1024;
	
	std::vector<FramebufferTextureConfig> Textures;
	std::vector<FramebufferRenderbufferConfig> Renderbuffers;
};

class Framebuffer
{
public:
	static Ref<Framebuffer> Create(const FramebufferConfig& config);

	Framebuffer(const FramebufferConfig& config);
	~Framebuffer();

	void Bind();

	void UpdateTarget(const FramebufferTextureConfig& textureConfig, int i = 0);

	void Unbind();

	void Resize(uint32_t width, uint32_t height);

	inline const FramebufferConfig& GetConfiguration() const { return m_Config; }

	inline uint32_t GetID() const { return m_ID; }
	inline uint32_t GetColorAttachment() const { return m_ColorAttachment; }
	inline uint32_t GetDepthAttachment() const { return m_DepthAttachment; }

private:
	uint32_t m_ID = 0;
	uint32_t m_ColorAttachment = 0;
	uint32_t m_DepthAttachment = 0;

	FramebufferConfig m_Config;
};