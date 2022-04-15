#pragma once

#include "Core.h"

class Texture;

class TextureImporter
{
public:
	TextureImporter();

	Ref<Texture> ImportTexture(std::string path);

private:
	std::unordered_map<std::string, Ref<Texture>> m_ImportedTextures;
};