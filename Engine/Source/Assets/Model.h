#pragma once

#include "Core.h"

class Mesh;

class Model
{
public:
	Model(std::vector<Ref<Mesh>> meshes);

	inline std::vector<Ref<Mesh>> GetMeshes() const { return m_Meshes; }

private:
	std::vector<Ref<Mesh>> m_Meshes;
};