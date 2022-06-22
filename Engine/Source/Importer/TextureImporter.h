#pragma once

#include "Core.h"
#include "Renderer/Texture.h"

class TextureImporter
{
public:
	TextureImporter();

	Ref<Texture> ImportTexture(std::string path, Texture::Type type, Texture::Wrap wrap);

private:
	std::unordered_map<std::string, Ref<Texture>> m_ImportedTextures;
};