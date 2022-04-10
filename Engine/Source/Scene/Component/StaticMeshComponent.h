#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer/StaticMesh.h"
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

	virtual void UpdateBoundingBox() override;
	virtual void UpdateBoundingSphere() override;

	virtual void Render(Material::BlendMode blendMode) override;

	virtual std::vector<Ref<Mesh>> GetMeshes() const override;
	virtual uint32_t GetRenderedVerticesCount() override;

private:
	std::vector<Ref<StaticMesh>> m_Meshes;
};