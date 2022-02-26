#pragma once

#include <unordered_map>
#include <string>
#include <mutex>

#include "typedefs.h"
#include "Material/Material.h"

class MaterialImporter
{
public:
	MaterialImporter();
	~MaterialImporter() {};

	MaterialImporter(MaterialImporter& other) = delete;
	void operator=(const MaterialImporter&) = delete;

	static Ref<MaterialImporter> GetInstance();

	Ref<Material> ImportMaterial(std::string path);

	void AddMaterial(std::string path, Ref<Material> material);

private:
	static Ref<MaterialImporter> s_Instance;
	static std::mutex s_Mutex;

	std::unordered_map<std::string, Ref<Material>> m_ImportedMaterials;
};