#pragma once

class Texture
{
public:
	Texture(std::string path, uint16_t width, uint16_t height, uint16_t componentsCount, uint8_t* data);
	Texture(std::string path, uint16_t width, uint16_t height, uint16_t componentsCount, float* data);
	~Texture();

	static Ref<Texture> Create(std::string path, uint16_t width, uint16_t height, uint16_t componentsCount, uint8_t* data);
	static Ref<Texture> CreateHDR(std::string path, uint16_t width, uint16_t height, uint16_t componentsCount, float* data);

	void Bind(uint32_t index);
	void Unbind();

	inline std::string GetPath() const { return m_Path; }

private:
	void Load(uint8_t* data);
	void LoadHDR(float* data);

private:
	std::string m_Path;
	uint32_t m_ID;
	
	uint16_t m_Width;
	uint16_t m_Height;
	uint16_t m_ComponentsCount;
};