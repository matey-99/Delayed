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

	void SetBoneMatrices(std::vector<glm::mat4> boneMatrices);

private:
	std::vector<Ref<SkeletalMesh>> m_Meshes;

	// Positions of bones sent to vertex shader and sent from Animator[Component]
	std::vector<glm::mat4> m_BoneMatrices;
};
