#include "RenderTarget.h"

Ref<RenderTarget> RenderTarget::Create(const RenderTarget::Config& config, uint32_t width, uint32_t height)
{
	return CreateRef<RenderTarget>(config, width, height);
}

Ref<RenderTarget> RenderTarget::CreateMRT(const std::vector<Config>& configs, uint32_t width, uint32_t height)
{
	return CreateRef<RenderTarget>(configs, width, height);
}

void RenderTarget::Copy(Ref<RenderTarget> source, Ref<RenderTarget> destination, Buffer buffer, Filter filter)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, source->m_ID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination->m_ID);

	glBlitFramebuffer(0, 0, source->m_Width, source->m_Height, 0, 0, destination->m_Width, destination->m_Height, (uint32_t)buffer, (uint32_t)filter);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

RenderTarget::RenderTarget(const Config& config, uint32_t width, uint32_t height)
	: m_Width(width), m_Height(height), m_ID(0)
{
	m_Configurations.push_back(config);

	Update(width, height);
}

RenderTarget::RenderTarget(const std::vector<Config>& configs, uint32_t width, uint32_t height)
	: m_Configurations(configs), m_Width(width), m_Height(height), m_ID(0)
{
	Update(width, height);
}

RenderTarget::~RenderTarget()
{
	glDeleteFramebuffers(1, &m_ID);
	glDeleteTextures(m_Targets.size(), &m_Targets[0]);
	glDeleteRenderbuffers(1, &m_DepthTarget);
}

void RenderTarget::Update(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;

	if (m_ID != 0)
	{
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(m_Targets.size(), &m_Targets[0]);
		glDeleteRenderbuffers(1, &m_DepthTarget);

		m_Targets.clear();
	}

	glGenFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

	bool hasDepth = false;
	int colorAttachmentIndex = 0;
	std::vector<uint32_t> colorAttachments;
	for (int i = 0; i < m_Configurations.size(); i++)
	{
		Config config = m_Configurations[i];
		if (config.Attachment == Attachment::Depth)
		{
			hasDepth = true;

			uint32_t type = (uint32_t)config.Type;
			uint32_t filter = (uint32_t)config.Filter;
			uint32_t internalFormat = (uint32_t)config.DepthInternalFormat;

			uint32_t attachment;
			switch (config.DepthInternalFormat)
			{
			case DepthInternalFormat::Depth:
			case DepthInternalFormat::Depth32:
			case DepthInternalFormat::Depth32F:
				attachment = GL_DEPTH_ATTACHMENT;
				break;
			case DepthInternalFormat::Depth24Stencil8:
				attachment = GL_DEPTH_STENCIL_ATTACHMENT;
				break;
			}

			m_Targets.push_back(0);
			glGenTextures(1, &m_Targets[i]);
			glBindTexture(type, m_Targets[i]);

			glTexParameteri(type, GL_TEXTURE_MIN_FILTER, filter);
			glTexParameteri(type, GL_TEXTURE_MAG_FILTER, filter);

			if (config.Type == Type::Texture2D)
			{
				glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

				float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				glTexParameterfv(type, GL_TEXTURE_BORDER_COLOR, borderColor);

				glTexStorage2D(type, 1, internalFormat, width, height);
				glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, type, m_Targets[i], 0);
			}
			else if (config.Type == Type::Texture2DArray)
			{
				glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

				float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				glTexParameterfv(type, GL_TEXTURE_BORDER_COLOR, borderColor);

				glTexStorage3D(type, 1, internalFormat, width, height, 5);
				glFramebufferTexture(GL_FRAMEBUFFER, attachment, m_Targets[i], 0);
			}
			else if (config.Type == Type::TextureCube)
			{
				glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

				glTexStorage2D(type, 1, internalFormat, width, height);
				glFramebufferTexture(GL_FRAMEBUFFER, attachment, m_Targets[i], 0);
			}
		}
		else if (config.Attachment == Attachment::Color)
		{
			uint32_t type = (uint32_t)config.Type;
			uint32_t filter = (uint32_t)config.Filter;
			uint32_t internalFormat = (uint32_t)config.ColorInternalFormat;
			uint32_t attachment = (uint32_t)config.Attachment;

			m_Targets.push_back(0);
			glGenTextures(1, &m_Targets[i]);
			glBindTexture(type, m_Targets[i]);

			glTexParameteri(type, GL_TEXTURE_MIN_FILTER, filter);
			glTexParameteri(type, GL_TEXTURE_MAG_FILTER, filter);

			if (config.Type == Type::Texture2D)
			{
				glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				glTexStorage2D(type, 1, internalFormat, width, height);
				glFramebufferTexture2D(GL_FRAMEBUFFER, attachment + colorAttachmentIndex, type, m_Targets[i], 0);
			}
			else if (config.Type == Type::Texture2DArray)
			{
				glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				glTexStorage3D(type, 1, internalFormat, width, height, 5);
				glFramebufferTexture2D(GL_FRAMEBUFFER, attachment + colorAttachmentIndex, type, m_Targets[i], 0);
			}
			else if (config.Type == Type::TextureCube)
			{
				glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

				glTexStorage2D(type, 1, internalFormat, width, height);
				glFramebufferTexture(GL_FRAMEBUFFER, attachment + colorAttachmentIndex, m_Targets[i], 0);
			}

			colorAttachments.push_back(attachment + colorAttachmentIndex);
			colorAttachmentIndex++;
		}
	}

	if (colorAttachments.size() > 0)
	{
		uint32_t* drawBuffers = &colorAttachments[0];
		glDrawBuffers(colorAttachmentIndex, drawBuffers);
	}
	else
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}


	if (!hasDepth)
	{
		glGenRenderbuffers(1, &m_DepthTarget);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthTarget);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthTarget);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer is incomplete: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	glViewport(0, 0, m_Width, m_Height);
}

void RenderTarget::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
