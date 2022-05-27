#pragma once

#include "Core.h"
#include "Assets/Font.h"

class FontImporter
{
public:
	FontImporter();

	Ref<Font> ImportFont(std::string path);

private:
	std::unordered_map<std::string, Ref<Font>> m_ImportedFonts;
};