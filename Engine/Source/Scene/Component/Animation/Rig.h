#pragma once
#include "Core.h"
#include "Renderer/Bone.h"

class Rig
{
public:
	Rig(std::vector<Ref<Bone>> bones)
		: m_Bones(bones) { }
	Rig() = default;
	//~Rig();

	// Add bone to rig
	// It will automatically assign new ID if didn't found such bone (based on name)
	void AddBone(Ref<Bone> bone);
	void CompleteBone(std::string boneName, aiNodeAnim* channel);

	inline size_t HowManyBones() const { return m_Bones.size(); }

	Ref<Bone> GetBone(uint32_t index) const;
	Ref<Bone> FindBone(std::string name) const;  // it's now basically a BoneInfo structure


private:
	std::vector<Ref<Bone>> m_Bones;
	glm::mat4 finalBoneMatrices;  // matrix that is sent to Vertex Shader, where is it computed?
};