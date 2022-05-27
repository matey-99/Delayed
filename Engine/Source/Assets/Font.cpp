#include "Font.h"

#include "Renderer/Texture.h"
#include "AssetManager.h"

#include <glad/glad.h>
#include <ft2build.h>
#include <freetype/freetype.h>

Font::Font(const std::string& path)
	: m_Path(path)
{
	Load(path);
}

Font::~Font()
{
}

Ref<Font> Font::Create(const std::string& path)
{
	return CreateRef<Font>(path);
}

void Font::Load(const std::string& path)
{
	m_Characters.clear();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		ENGINE_ERROR("FreeType error: could not initialize FreeType Library!");
		return;
	}

	FT_Face face;
	if (FT_New_Face(ft, (AssetManager::ContentDirectory + path).c_str(), 0, &face))
	{
		ENGINE_ERROR("FreeType error: failed to load font from: " + path);
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, FONT_SIZE);

	for (uint8_t c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			ENGINE_WARN("FreeType warning: failed to load glyph for character: " + c);
			continue;
		}

		uint32_t texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character;
		character.ID = texture;
		character.Size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		character.Bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
		character.Advance = face->glyph->advance.x;

		m_Characters.insert({ c, character });
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

const Font::Character& Font::GetCharacter(uint8_t character) const
{
	return m_Characters.find(character)->second;
}
