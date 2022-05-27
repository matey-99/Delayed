#include "FontImporter.h"

#include "Assets/AssetManager.h"

FontImporter::FontImporter()
{
}

Ref<Font> FontImporter::ImportFont(std::string path)
{
	if (m_ImportedFonts.find(path) != m_ImportedFonts.end())
		return m_ImportedFonts.at(path);

	std::string extension = path.substr(path.find_last_of(".") + 1);
	std::string relativePath = path.substr(AssetManager::ContentDirectory.size());
	Ref<Font> font = Font::Create(relativePath);
	if (!font)
		return Ref<Font>();

	m_ImportedFonts.insert({ path, font });
	return font;
}
