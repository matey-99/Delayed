#pragma once

#include "Core.h"
#include "Material/Material.h"

class MaterialInstance : public Material
{
public:
	MaterialInstance(Ref<Material> material);

	static Ref<MaterialInstance> Create(Ref<Material> material);

	void SetBoolParameter(std::string name, bool value);
	void SetFloatParameter(std::string name, float value);
	void SetVec3Parameter(std::string name, glm::vec3 value);
	void SetTexture2DParameter(std::string name, Ref<Texture> value);
};