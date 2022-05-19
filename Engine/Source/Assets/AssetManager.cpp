#include "AssetManager.h"

#include "Importer/ModelImporter.h"
#include "Importer/SkeletalModelImporter.h"
#include "Importer/AnimationImporter.h"
#include "Importer/TextureImporter.h"
#include "Importer/SceneImporter.h"
#include "Importer/MaterialImporter.h"
#include "Importer/ShaderImporter.h"
#include "Importer/ComputeShaderImporter.h"

#include "Scene/SceneSerializer.h"
#include "Material/MaterialSerializer.h"

const std::string AssetManager::ContentDirectory = "../../../Content/";

AssetManager::AssetManager()
{
	m_ModelImporter = CreateRef<ModelImporter>();
	m_SkeletalModelImporter = CreateRef<SkeletalModelImporter>();
	m_AnimationImporter = CreateRef<AnimationImporter>();
	m_TextureImporter = CreateRef<TextureImporter>();
	m_ShaderImporter = CreateRef<ShaderImporter>();
	m_ComputeShaderImporter = CreateRef<ComputeShaderImporter>();

	m_SceneImporter = CreateRef<SceneImporter>();
	m_MaterialImporter = CreateRef<MaterialImporter>();
}

Ref<Model> AssetManager::LoadModel(std::string assetPath)
{
	std::string fullPath = ContentDirectory + assetPath;
	return GetInstance()->m_ModelImporter->ImportModel(fullPath);
}

Ref<SkeletalModel> AssetManager::LoadSkeletalModel(std::string assetPath)
{
	std::string fullPath = ContentDirectory + assetPath;
	return GetInstance()->m_SkeletalModelImporter->ImportSkeletalModel(fullPath);
}

std::vector<Ref<Animation>> AssetManager::LoadAnimations(std::string assetPath, Ref<Rig> rig)
{
	std::string fullPath = ContentDirectory + assetPath;
	return GetInstance()->m_AnimationImporter->ImportAnimations(fullPath, rig);
}

Ref<Texture> AssetManager::LoadTexture(std::string assetPath, Texture::Type type)
{
	std::string fullPath = ContentDirectory + assetPath;
	return GetInstance()->m_TextureImporter->ImportTexture(fullPath, type);
}

Ref<Shader> AssetManager::LoadShader(const std::string& path)
{
	std::string fullPath = ContentDirectory + path;
	return GetInstance()->m_ShaderImporter->ImportShader(fullPath);
}

Ref<ComputeShader> AssetManager::LoadComputeShader(const std::string& path)
{
	std::string fullPath = ContentDirectory + path;
	return GetInstance()->m_ComputeShaderImporter->ImportComputeShader(fullPath);
}

Ref<Scene> AssetManager::LoadScene(std::string assetPath)
{
	std::string fullPath = ContentDirectory + assetPath;
	return GetInstance()->m_SceneImporter->ImportScene(fullPath);
}

Ref<Material> AssetManager::LoadMaterial(std::string assetPath)
{
	std::string fullPath = ContentDirectory + assetPath;
	return GetInstance()->m_MaterialImporter->ImportMaterial(fullPath);
}

void AssetManager::CreateNewMaterial(std::string name, std::string destinationPath, std::string shaderName)
{
	std::string fullPath = ContentDirectory + destinationPath;
	auto material = Material::Create(name, destinationPath, shaderName);

	MaterialSerializer::Serialize(material, fullPath);
	GetInstance()->m_MaterialImporter->AddMaterial(fullPath, material);
}

void AssetManager::SaveMaterial(Ref<Material> material)
{
	std::string fullPath = ContentDirectory + material->GetPath();

	MaterialSerializer::Serialize(material, fullPath);
}
