#include "SkeletalModel.h"
#include "Renderer/SkeletalMesh.h"

SkeletalModel::SkeletalModel(const std::string& path, std::vector<Ref<SkeletalMesh>> meshes, Ref<Rig> rig,
	glm::mat4 globalInverseTransform)
	: ModelBase(path), m_Meshes(meshes), m_Rig(rig), m_GlobalInverseTransform(globalInverseTransform)
{

}

Ref<SkeletalModel> SkeletalModel::Create(const std::string& path, std::vector<Ref<SkeletalMesh>> meshes, Ref<Rig> rig,
	glm::mat4 globalInverseTransform)
{
	return CreateRef<SkeletalModel>(path, meshes, rig, globalInverseTransform);
}

void SkeletalModel::PropagateBoneTransforms(std::vector<glm::mat4> boneMatrices)
{
	for (auto& mesh : m_Meshes)
	{
		mesh->SetBoneMatrices(boneMatrices);
	}
}
