#pragma once

#include "Core.h"
#include "Patterns/Singleton.h"

class Model;
class SkeletalModel;
class Texture;
class Shader;
class ComputeShader;
class Scene;
class Material;

class ModelImporter;
class SkeletalModelImporter;
class TextureImporter;
class ShaderImporter;
class ComputeShaderImporter;
class SceneImporter;
class MaterialImporter;

class AssetManager : Singleton<AssetManager>
{
public:
	AssetManager();

	static Ref<Model> LoadModel(std::string assetPath);
	static Ref<SkeletalModel> LoadSkeletalModel(std::string assetPath);
	static Ref<Texture> LoadTexture(std::string assetPath);
	static Ref<Shader> LoadShader(const std::string& path);
	static Ref<ComputeShader> LoadComputeShader(const std::string& path);

	static Ref<Scene> LoadScene(std::string assetPath);
	static Ref<Material> LoadMaterial(std::string assetPath);

	static void CreateNewMaterial(std::string name, std::string destinationPath, std::string shaderName = "Opaque");
	static void SaveMaterial(Ref<Material> material);

	static const std::string ContentDirectory;

private:
	Ref<ModelImporter> m_ModelImporter;
	Ref<SkeletalModelImporter> m_SkeletalModelImporter;
	Ref<TextureImporter> m_TextureImporter;
	Ref<ShaderImporter> m_ShaderImporter;
	Ref<ComputeShaderImporter> m_ComputeShaderImporter;
	Ref<SceneImporter> m_SceneImporter;
	Ref<MaterialImporter> m_MaterialImporter;
};