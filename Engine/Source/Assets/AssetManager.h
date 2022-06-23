#pragma once

#include "Core.h"
#include "Patterns/Singleton.h"
#include "Renderer/Texture.h"

class Model;
class SkeletalModel;
class Rig;
class Animation;
class Shader;
class ComputeShader;
class Scene;
class Material;
class Font;

class ModelImporter;
class SkeletalModelImporter;
class AnimationImporter;
class TextureImporter;
class ShaderImporter;
class ComputeShaderImporter;
class SceneImporter;
class MaterialImporter;
class FontImporter;

class AssetManager : Singleton<AssetManager>
{
public:
	AssetManager();

	static Ref<Model> LoadModel(std::string assetPath);
	static Ref<SkeletalModel> LoadSkeletalModel(std::string assetPath);
	static std::vector<Ref<Animation>> LoadAnimations(std::string assetPath, Ref<Rig> rig);
	static Ref<Texture> LoadTexture(std::string assetPath, Texture::Type type = Texture::Type::BaseColor, Texture::Wrap wrap = Texture::Wrap::Repeat);
	static Ref<Font> LoadFont(const std::string& assetPath);
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
	Ref<AnimationImporter> m_AnimationImporter;
	Ref<TextureImporter> m_TextureImporter;
	Ref<FontImporter> m_FontImporter;
	Ref<ShaderImporter> m_ShaderImporter;
	Ref<ComputeShaderImporter> m_ComputeShaderImporter;
	Ref<SceneImporter> m_SceneImporter;
	Ref<MaterialImporter> m_MaterialImporter;
};