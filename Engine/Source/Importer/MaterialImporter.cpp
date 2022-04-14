#include "MaterialImporter.h"

#include "Material/MaterialSerializer.h"

Ref<Material> MaterialImporter::ImportMaterial(std::string path)
{
	if (m_ImportedMaterials.find(path) != m_ImportedMaterials.end())
		return m_ImportedMaterials.at(path);

	Ref<Material> material = MaterialSerializer::Deserialize(path);
	if (!material)
		return Ref<Material>();

	m_ImportedMaterials.insert({ path, material });
	return material;
}

void MaterialImporter::AddMaterial(std::string path, Ref<Material> material)
{
	m_ImportedMaterials.insert({ path, material });
}
