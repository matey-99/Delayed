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
	Ref<Animation> GetAnimation(int index);
	std::vector<Ref<Animation>> GetAnimations() { return m_Animations; }
	uint32_t GetBoneCount() const;
	uint32_t HowManyAnimations() { return m_Animations.size(); }

	Ref<BoneMap> FindBoneInRig(std::string boneName)
	{
		return m_SkeletalModel->GetRig()->FindBone(boneName);
	}

	void PropagateBoneTransforms(std::vector<glm::mat4> boneMatrices);


private:
	Ref<SkeletalModel> m_SkeletalModel;
	std::vector<Ref<Animation>> m_Animations;

};