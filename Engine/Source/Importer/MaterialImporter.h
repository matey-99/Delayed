#pragma once

#include "Core.h"
#include "Patterns/Singleton.h"

#include "Material/Material.h"

class MaterialImporter : public Singleton<MaterialImporter>
{
public:
	Ref<Material> ImportMaterial(std::string path);

	void AddMaterial(std::string path, Ref<Material> material);

private:
	std::unordered_map<std::string, Ref<Material>> m_ImportedMaterials;
};