#include "SkeletalModel.h"
#include "Renderer/SkeletalMesh.h"

SkeletalModel::SkeletalModel(const std::string& path, std::vector<Ref<SkeletalMesh>> meshes, Ref<Rig> rig)
	: ModelBase(path), m_Meshes(meshes), m_Rig(rig)
{

}

Ref<SkeletalModel> SkeletalModel::Create(const std::string& path, std::vector<Ref<SkeletalMesh>> meshes, Ref<Rig> rig)
{
	return CreateRef<SkeletalModel>(path, meshes, rig);
}

void SkeletalModel::PropagateBoneTransforms(std::vector<glm::mat4> boneMatrices)
{
	for (auto& mesh : m_Meshes)
	{
		mesh->SetBoneMatrices(boneMatrices);
	}
}
