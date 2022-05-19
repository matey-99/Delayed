#pragma once

#include "Core.h"
#include "ModelBase.h"
#include "Scene/Component/Animation/Rig.h"

class Animation;
class SkeletalMesh;

class SkeletalModel : public ModelBase
{
public:
	SkeletalModel(const std::string& path, std::vector<Ref<SkeletalMesh>> meshes, Ref<Rig> rig);

	static Ref<SkeletalModel> Create(const std::string& path,
		std::vector<Ref<SkeletalMesh>> meshes,
		Ref<Rig> rig
	);

	inline std::vector<Ref<SkeletalMesh>> GetMeshes() const { return m_Meshes; }

	Ref<Rig> GetRig() const { return m_Rig; }

	void PropagateBoneTransforms(std::vector<glm::mat4> boneMatrices);

	/*void AddAnimation(Ref<Animation> animation) { m_Animations.push_back(animation); }
	std::vector<Ref<Animation>> GetAnimations() const { return m_Animations; }*/

private:
	std::vector<Ref<SkeletalMesh>> m_Meshes;
	//std::vector<Ref<Animation>> m_Animations;
	Ref<Rig> m_Rig;

	// Positions of bones sent to vertex shader and sent from Animator[Component]
	//std::vector<glm::mat4> m_BoneMatrices;
};