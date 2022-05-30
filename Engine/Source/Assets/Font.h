#pragma once

#include "Core.h"

#define FONT_SIZE 512

class Font
{
public:
	struct Character
	{
		uint32_t ID;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		uint32_t Advance;
	};

public:
	Font(const std::string& path);
	~Font();

	static Ref<Font> Create(const std::string& path);

	void Load(const std::string& path);
	const Character& GetCharacter(uint8_t character) const;

	inline const std::string& GetPath() const { return m_Path; }

private:
	std::string m_Path;
	std::unordered_map<uint8_t, Character> m_Characters;
};