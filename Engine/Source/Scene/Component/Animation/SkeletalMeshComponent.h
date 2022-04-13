#pragma once

#include "Renderer/SkeletalMesh.h"
#include "Material/Material.h"
#include "Material/ShaderLibrary.h"
#include "Scene/Component/MeshComponent.h"

class SkeletalMeshComponent : public MeshComponent
{


public:
	SkeletalMeshComponent(Actor* owner);
	SkeletalMeshComponent(Actor* owner, std::string path);
	SkeletalMeshComponent(Actor* owner, std::string path, std::vector<std::string> materialsPath);

	void LoadMesh(std::string path);
	void ChangeMesh(std::string path);

	virtual void UpdateBoundingBox() override;
	virtual void UpdateBoundingSphere() override;

	virtual void Render(Material::BlendMode blendMode) override;

	virtual std::vector<Ref<Mesh>> GetMeshes() const override;
	virtual uint32_t GetRenderedVerticesCount() override;

	// Details
	uint32_t GetBoneCount();

private:
	std::vector<Ref<SkeletalMesh>> m_Meshes;
};