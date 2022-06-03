#pragma once

#include "Core.h"
#include "Material/Material.h"

class MaterialInstance : public Material
{
public:
	MaterialInstance(Ref<Material> material);

	static Ref<MaterialInstance> Create(Ref<Material> material);

	bool GetBoolParameter(std::string name);
	float GetFloatParameter(std::string name);
	glm::vec3 GetVec3Parameter(std::string name);
	Ref<Texture> GetTexture2DParameter(std::string name);

	void SetBoolParameter(std::string name, bool value);
	void SetFloatParameter(std::string name, float value);
	void SetVec3Parameter(std::string name, glm::vec3 value);
	void SetTexture2DParameter(std::string name, Ref<Texture> value);
};