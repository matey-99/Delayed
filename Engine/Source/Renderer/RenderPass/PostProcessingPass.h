#pragma once

enum class TextureRange
{
	LDR, HDR
};

class Texture
{
public:
	Texture(std::string path, TextureRange range);
	~Texture();

	static Ref<Texture> Create(std::string path, TextureRange range = TextureRange::LDR);

	void Load(std::string path);
	void LoadHDR(std::string path);

	void Bind(uint32_t index);
	void Unbind();

	inline std::string GetPath() const { return m_Path; }

private:
	uint32_t m_ID;
	std::string m_Path;
};