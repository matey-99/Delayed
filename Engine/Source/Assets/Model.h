#pragma once

#include "Core.h"
#include "ModelBase.h"

class Mesh;

class Model : public ModelBase
{
public:
	Model(const std::string& path, std::vector<Ref<Mesh>> meshes);

	static Ref<Model> Create(const std::string& path, std::vector<Ref<Mesh>> meshes);

	inline std::vector<Ref<Mesh>> GetMeshes() const { return m_Meshes; }

private:
	std::vector<Ref<Mesh>> m_Meshes;
};