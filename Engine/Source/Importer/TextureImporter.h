#pragma once

#include "Core.h"
#include "Patterns/Singleton.h"

#include "Material/Material.h"

class TextureImporter : public Singleton<TextureImporter>
{
public:
	Ref<Texture> ImportTexture(std::string path);

private:
	std::unordered_map<std::string, Ref<Texture>> m_ImportedTextures;
};