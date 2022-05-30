#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "typedefs.h"

class RenderTarget
{
public:
	enum class Attachment
	{
		Color = GL_COLOR_ATTACHMENT0,
		Depth = GL_DEPTH_ATTACHMENT
	};

	enum class Type
	{
		Texture2D = GL_TEXTURE_2D,
		Texture2DArray = GL_TEXTURE_2D_ARRAY,
		TextureCube = GL_TEXTURE_CUBE_MAP
	};

	enum class ColorInternalFormat
	{
		R8 = GL_R8,
		R16 = GL_R16,
		R32F = GL_R32F,
		RG8 = GL_RG8,
		RG16 = GL_RG16,
		RG16F = GL_RG16F,
		RGB8 = GL_RGB8,
		RGB12 = GL_RGB12,
		RGBA8 = GL_RGBA8,
		RGBA16 = GL_RGBA16,
		RGBA16F = GL_RGBA16F,
		RGBA32F = GL_RGBA32F
	};

	enum class DepthInternalFormat
	{
		Depth = GL_DEPTH_COMPONENT,
		Depth32 = GL_DEPTH_COMPONENT32,
		Depth32F = GL_DEPTH_COMPONENT32F,
		Depth24Stencil8 = GL_DEPTH24_STENCIL8
	};

	enum class Filter
	{
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR
	};

	enum class Buffer
	{
		Depth = GL_DEPTH_BUFFER_BIT,
		Color = GL_COLOR_BUFFER_BIT
	};

	struct Config
	{
		Attachment Attachment = Attachment::Color;
		Type Type = Type::Texture2D;
		ColorInternalFormat ColorInternalFormat = ColorInternalFormat::RGBA32F;
		DepthInternalFormat DepthInternalFormat = DepthInternalFormat::Depth;
		Filter Filter = Filter::Nearest;
	};


public:
	static Ref<RenderTarget> Create(const Config& config, uint32_t width, uint32_t height);
	static Ref<RenderTarget> CreateMRT(const std::vector<Config>& configs, uint32_t width, uint32_t height);

	static void Copy(Ref<RenderTarget> source, Ref<RenderTarget> destination, Buffer buffer, Filter filter);

	RenderTarget(const Config& config, uint32_t width, uint32_t height);
	RenderTarget(const std::vector<Config>& configs, uint32_t width, uint32_t height);
	~RenderTarget();

	void Update(uint32_t width, uint32_t height);

	void Bind();
	void Unbind();

	inline const Config& GetConfiguration(int index) const { return m_Configurations.at(index); }

	inline uint32_t GetID() const { return m_ID; }
	inline uint32_t GetDepthTarget() const { return m_DepthTarget; }
	inline std::vector<uint32_t> GetTargets() const { return m_Targets; }
	
	inline uint32_t GetWidth() const { return m_Width; }
	inline uint32_t GetHeight() const { return m_Height; }

private:
	uint32_t m_ID;

	uint32_t m_DepthTarget;
	std::vector<uint32_t> m_Targets;

	std::vector<Config> m_Configurations;
	uint32_t m_Width;
	uint32_t m_Height;
};