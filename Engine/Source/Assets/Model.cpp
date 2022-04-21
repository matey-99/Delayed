#include "Model.h"

Model::Model(const std::string& path, std::vector<Ref<Mesh>> meshes)
	: ModelBase(path), m_Meshes(meshes)
{

}

Ref<Model> Model::Create(const std::string& path, std::vector<Ref<Mesh>> meshes)
{
	return CreateRef<Model>(path, meshes);
}
