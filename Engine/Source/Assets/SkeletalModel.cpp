#include "SkeletalModel.h"
#include "../Renderer/SkeletalMesh.h"

SkeletalModel::SkeletalModel(const std::string& path, std::vector<Ref<SkeletalMesh>> meshes, Ref<Rig> rig)
	: ModelBase(path), m_Meshes(meshes)
{
	//m_BoneMatrices.reserve(100);

	/*for (int i = 0; i < 100; i++)
		m_BoneMatrices.push_back(glm::mat4(1.0f));*/
}

Ref<SkeletalModel> SkeletalModel::Create(const std::string& path, std::vector<Ref<SkeletalMesh>> meshes, Ref<Rig> rig)
{
	return CreateRef<SkeletalModel>(path, meshes, rig);
}

//void SkeletalModel::SetBoneMatrices(std::vector<glm::mat4> boneMatrices)
//{
//	// set for itself
//	m_BoneMatrices = std::vector<glm::mat4>(boneMatrices);
//	
//	// set into SkeletalMesh
//	for (auto& skelMesh : m_Meshes)
//	{
//		skelMesh->SetBoneMatrices(m_BoneMatrices);
//	}
//
//}
