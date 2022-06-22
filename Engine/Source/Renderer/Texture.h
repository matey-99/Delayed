#pragma once

#include <glad/glad.h>

class Texture
{
public:
	enum class Type
	{
		BaseColor	= 0, 
		Normal		= 1,
		Roughness	= 2,
		Metallic	= 3,
		AO			= 4,
		Emissive	= 5,
		Opacity		= 6
	};

	enum class Wrap
	{
		Repeat = GL_REPEAT,
		ClampToEdge = GL_CLAMP_TO_EDGE
	};

public:
	Texture(std::string path, Type type, uint16_t width, uint16_t height, uint16_t componentsCount, uint8_t* data, Wrap wrap);
	Texture(std::string path, uint16_t width, uint16_t height, uint16_t componentsCount, float* data);
	~Texture();

	static Ref<Texture> Create(std::string path, Type type, uint16_t width, uint16_t height, uint16_t componentsCount, uint8_t* data, Wrap wrap = Wrap::Repeat);
	static Ref<Texture> CreateHDR(std::string path, uint16_t width, uint16_t height, uint16_t componentsCount, float* data);

	void Bind(uint32_t index);
	void Unbind();

	inline std::string GetPath() const { return m_Path; }
	inline Type GetType() const { return m_Type; }

private:
	void Load(uint8_t* data, Type type, Wrap wrap = Wrap::Repeat);
	void LoadHDR(float* data);

private:
	std::string m_Path;
	uint32_t m_ID;
	
	Type m_Type;
	uint16_t m_Width;
	uint16_t m_Height;
	uint16_t m_ComponentsCount;
};