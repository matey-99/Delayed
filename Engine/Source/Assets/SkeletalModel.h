#pragma once

#include "Core.h"
#include "ModelBase.h"

class SkeletalMesh;

class SkeletalModel : public ModelBase
{
public:
	SkeletalModel(const std::string& path, std::vector<Ref<SkeletalMesh>> meshes);

	static Ref<SkeletalModel> Create(const std::string& path, std::vector<Ref<SkeletalMesh>> meshes);

	inline std::vector<Ref<SkeletalMesh>> GetMeshes() const { return m_Meshes; }

private:
	std::vector<Ref<SkeletalMesh>> m_Meshes;
};