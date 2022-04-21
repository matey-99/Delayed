#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Assets/Model.h"
#include "Material/Material.h"
#include "Material/ShaderLibrary.h"
#include "Scene/Component/MeshComponent.h"

class StaticMeshComponent : public MeshComponent
{
public:
	StaticMeshComponent(Actor* owner);
	StaticMeshComponent(Actor* owner, std::string path);
	StaticMeshComponent(Actor* owner, std::string path, std::vector<std::string> materialsPath);

	virtual void LoadMesh(std::string path) override;
	virtual void ChangeMesh(std::string path) override;
	virtual void ChangeModel(Ref<ModelBase> modelBase) override;
	virtual Ref<ModelBase> GetModel() const override;

	virtual void UpdateBoundingBox() override;
	virtual void UpdateBoundingSphere() override;

	virtual void Render(Material::BlendMode blendMode) override;

	virtual std::vector<Ref<MeshBase>> GetMeshes() const override;
	virtual uint32_t GetRenderedVerticesCount() override;

private:
	Ref<Model> m_Model;
};