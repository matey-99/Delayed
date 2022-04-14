#include "AssetManager.h"

#include "Importer/ModelImporter.h"
#include "Importer/SkeletalModelImporter.h"
#include "Importer/TextureImporter.h"
#include "Importer/MaterialImporter.h"

#include "Scene/SceneSerializer.h"
#include "Material/MaterialSerializer.h"

const std::string AssetManager::ContentDirectory = "../../../Content/";

Ref<Model> AssetManager::LoadModel(std::string assetPath)
{
	std::string fullPath = "../../../Content/" + assetPath;
	return ModelImporter::GetInstance()->ImportModel(fullPath);
}

Ref<SkeletalModel> AssetManager::LoadSkeletalModel(std::string assetPath)
{
	std::string fullPath = "../../../Content/" + assetPath;
	return SkeletalModelImporter::GetInstance()->ImportSkeletalModel(fullPath);
}

Ref<Texture> AssetManager::LoadTexture(std::string assetPath)
{
	std::string fullPath = "../../../Content/" + assetPath;
	return TextureImporter::GetInstance()->ImportTexture(fullPath);
}

Ref<Shader> AssetManager::LoadShader(std::string name, std::string vertexPath, std::string fragmentPath, std::string geometryPath)
{
	std::string fullVertexPath = "../../../Content/" + vertexPath;
	std::string fullFragmentPath = "../../../Content/" + fragmentPath;
	std::string fullGeometryPath = "";
	if (geometryPath != "")
		fullGeometryPath = "../../../Content/" + geometryPath;

	return CreateRef<Shader>(name, fullVertexPath, fullFragmentPath, fullGeometryPath);
}

Ref<ComputeShader> AssetManager::LoadComputeShader(std::string name, std::string assetPath)
{
	std::string fullPath = "../../../Content/" + assetPath;
	return CreateRef<ComputeShader>(fullPath);
}

Ref<Scene> AssetManager::LoadScene(std::string assetPath)
{
	std::string fullPath = "../../../Content/" + assetPath;
	return SceneSerializer::Deserialize(fullPath);
}

Ref<Material> AssetManager::LoadMaterial(std::string assetPath)
{
	std::string fullPath = "../../../Content/" + assetPath;
	return MaterialImporter::GetInstance()->ImportMaterial(fullPath);
}

void AssetManager::CreateMaterial(std::string name, Ref<Material> material)
{
	MaterialSerializer::Serialize(material, "../../../Content/Materials/");
	MaterialImporter::GetInstance()->AddMaterial("../../../Content/Materials/" + material->GetName() + ".mat", material);
}
