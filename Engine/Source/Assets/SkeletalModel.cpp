#include "SkeletalModel.h"

SkeletalModel::SkeletalModel(const std::string& path, std::vector<Ref<SkeletalMesh>> meshes)
	: ModelBase(path), m_Meshes(meshes)
{

}

Ref<SkeletalModel> SkeletalModel::Create(const std::string& path, std::vector<Ref<SkeletalMesh>> meshes)
{
	return CreateRef<SkeletalModel>(path, meshes);
}
