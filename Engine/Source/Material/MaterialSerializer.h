#pragma once

#include "Material.h"
#include "ShaderLibrary.h"

class MaterialSerializer
{
public:
	static void Serialize(Ref<Material> material);
	static Ref<Material> Deserialize(std::string path);
};