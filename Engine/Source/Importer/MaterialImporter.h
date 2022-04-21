#pragma once

#include "Core.h"

class Material;

class MaterialImporter
{
public:
	MaterialImporter();

	Ref<Material> ImportMaterial(std::string path);

	void AddMaterial(std::string path, Ref<Material> material);

private:
	std::unordered_map<std::string, Ref<Material>> m_ImportedMaterials;
};