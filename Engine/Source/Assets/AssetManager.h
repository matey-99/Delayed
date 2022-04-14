#pragma once

#include "Core.h"

class Model;
class SkeletalModel;
class Texture;
class Shader;
class ComputeShader;

class Scene;
class Material;

class AssetManager
{
public:
	static Ref<Model> LoadModel(std::string assetPath);
	static Ref<SkeletalModel> LoadSkeletalModel(std::string assetPath);
	static Ref<Texture> LoadTexture(std::string assetPath);
	static Ref<Shader> LoadShader(std::string name, std::string vertexPath, std::string fragmentPath, std::string geometryPath = "");
	static Ref<ComputeShader> LoadComputeShader(std::string name, std::string assetPath);

	static Ref<Scene> LoadScene(std::string assetPath);
	static Ref<Material> LoadMaterial(std::string assetPath);

	static void CreateMaterial(std::string name, Ref<Material> material);

	static const std::string ContentDirectory;
};