#include "MaterialImporter.h"

#include "Material/MaterialSerializer.h"

Ref<MaterialImporter> MaterialImporter::s_Instance{};
std::mutex MaterialImporter::s_Mutex;

MaterialImporter::MaterialImporter()
{
	m_ImportedMaterials = std::unordered_map<std::string, Ref<Material>>();
}

Ref<MaterialImporter> MaterialImporter::GetInstance()
{
	std::lock_guard<std::mutex> lock(s_Mutex);
	if (s_Instance == nullptr)
		s_Instance = CreateRef<MaterialImporter>();

	return s_Instance;
}

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
