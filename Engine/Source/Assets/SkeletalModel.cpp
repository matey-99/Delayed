#include "SkeletalModel.h"

SkeletalModel::SkeletalModel(const std::string& path, std::vector<Ref<SkeletalMesh>> meshes)
	: ModelBase(path), m_Meshes(meshes)
{
	m_BoneMatrices.reserve(100);

	for (int i = 0; i < 100; i++)
		m_BoneMatrices.push_back(glm::mat4(1.0f));
}

Ref<SkeletalModel> SkeletalModel::Create(const std::string& path, std::vector<Ref<SkeletalMesh>> meshes)
{
	return CreateRef<SkeletalModel>(path, meshes);
}

void SkeletalModel::SetBoneMatrices(std::vector<glm::mat4> boneMatrices)
{
	m_BoneMatrices = boneMatrices;
}
