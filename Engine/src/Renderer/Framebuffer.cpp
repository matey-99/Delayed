#include "Framebuffer.h"

#include <glad/glad.h>
#include "Renderer.h"

Ref<Framebuffer> Framebuffer::Create(const FramebufferConfig& config)
{
	return CreateRef<Framebuffer>(config);
}

Framebuffer::Framebuffer(const FramebufferConfig& config)
	: m_Config(config)
{
	Resize(m_Config.Width, m_Config.Height);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_ID);
	glDeleteTextures(1, &m_ColorAttachment);
	glDeleteRenderbuffers(1, &m_DepthAttachment);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	glViewport(0, 0, m_Config.Width, m_Config.Height);
}

void Framebuffer::UpdateTarget(const FramebufferTextureConfig& textureConfig, int i)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, textureConfig.Attachment, textureConfig.Target + i, m_ColorAttachment, 0);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(uint32_t width, uint32_t height)
{
	m_Config.Width = width;
	m_Config.Height = height;

	if (m_ID)
	{
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteRenderbuffers(1, &m_DepthAttachment);
	}

	glGenFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

	if (m_Config.Textures.size())
	{
		for (auto& textureConfig : m_Config.Textures)
		{
			uint32_t* attachment;
			if (textureConfig.Attachment == GL_COLOR_ATTACHMENT0)
			{
				attachment = &m_ColorAttachment;
			}
			else if (textureConfig.Attachment = GL_DEPTH_ATTACHMENT)
			{
				attachment = &m_DepthAttachment;
			}

			glGenTextures(1, attachment);
			glBindTexture(textureConfig.Target, *attachment);
			glTexImage2D(textureConfig.Target, 0, textureConfig.InternalFormat, m_Config.Width, m_Config.Height, 0, textureConfig.Format, textureConfig.Type, nullptr);

			glTexParameteri(textureConfig.Target, GL_TEXTURE_MIN_FILTER, textureConfig.MinFilter);
			glTexParameteri(textureConfig.Target, GL_TEXTURE_MAG_FILTER, textureConfig.MagFilter);

			if (textureConfig.WrapS != GL_NONE && textureConfig.WrapT != GL_NONE)
			{
				glTexParameteri(textureConfig.Target, GL_TEXTURE_WRAP_S, textureConfig.WrapS);
				glTexParameteri(textureConfig.Target, GL_TEXTURE_WRAP_T, textureConfig.WrapT);
			}

			if (textureConfig.Border)
			{
				float border[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				glTexParameterfv(textureConfig.Target, GL_TEXTURE_BORDER_COLOR, border);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, textureConfig.Attachment, textureConfig.Target, *attachment, 0);
		}
	}
	
	if (!m_ColorAttachment)
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	if (m_Config.Renderbuffers.size())
	{
		for (auto& renderbufferConfig : m_Config.Renderbuffers)
		{
			glGenRenderbuffers(1, &m_DepthAttachment);
			glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment);
			glRenderbufferStorage(GL_RENDERBUFFER, renderbufferConfig.InternalFormat, m_Config.Width, m_Config.Height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, renderbufferConfig.Attachment, GL_RENDERBUFFER, m_DepthAttachment);
		}
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer is incomplete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
