#pragma once

#include "Core.h"

class SkeletalMesh;

class SkeletalModel
{
public:
	SkeletalModel(std::vector<Ref<SkeletalMesh>> meshes);

	inline std::vector<Ref<SkeletalMesh>> GetMeshes() const { return m_Meshes; }

private:
	std::vector<Ref<SkeletalMesh>> m_Meshes;
};