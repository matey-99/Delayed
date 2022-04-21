#pragma once

#include "Renderer/SkeletalMesh.h"
#include "Assets/SkeletalModel.h"
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
	virtual void ChangeModel(Ref<ModelBase> modelBase) override;
	virtual Ref<ModelBase> GetModel() const override;

	virtual void UpdateBoundingBox() override;
	virtual void UpdateBoundingSphere() override;

	virtual void Render(Material::BlendMode blendMode) override;

	virtual std::vector<Ref<MeshBase>> GetMeshes() const override;
	virtual uint32_t GetRenderedVerticesCount() override;
	std::vector<Ref<Animation>> GetAnimations();

	// Sets Bones Position to SkeletalModel
	void SetBonesPositionInSkeletalModel(std::vector<glm::mat4> boneMatrices);

	// Details
	uint32_t GetBoneCount();

private:
	Ref<SkeletalModel> m_SkeletalModel;
};
