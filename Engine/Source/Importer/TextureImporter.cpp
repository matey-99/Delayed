#include "TextureImporter.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Assets/AssetManager.h"

TextureImporter::TextureImporter()
{
}

Ref<Texture> TextureImporter::ImportTexture(std::string path, Texture::Type type)
{
	if (m_ImportedTextures.find(path) != m_ImportedTextures.end())
		return m_ImportedTextures.at(path);

	std::string extension = path.substr(path.find_last_of(".") + 1);
	std::string relativePath = path.substr(AssetManager::ContentDirectory.size());
	Ref<Texture> importedTexture;
	if (extension == "png" || extension == "jpg")
	{
		stbi_set_flip_vertically_on_load(true);
		int width, height, componentsCount;
		uint8_t* data = stbi_load(path.c_str(), &width, &height, &componentsCount, 0);
		if (data)
		{
			importedTexture = Texture::Create(relativePath, type, width, height, componentsCount, data);
		}
		else
		{
			std::cout << "Error: Loading texture from: " << path << " failed" << std::endl;
		}

		stbi_image_free(data);
	}
	else if (extension == "hdr" || extension == "ext")
	{
		stbi_set_flip_vertically_on_load(true);
		int width, height, componentsCount;
		float* data = stbi_loadf(path.c_str(), &width, &height, &componentsCount, 0);
		if (data)
		{
			importedTexture = Texture::CreateHDR(relativePath, width, height, componentsCount, data);
		}
		else
		{
			std::cout << "Error: Loading texture from: " << path << " failed" << std::endl;
		}

		stbi_image_free(data);
	}

	m_ImportedTextures.insert({ path, importedTexture });
	return importedTexture;
}
