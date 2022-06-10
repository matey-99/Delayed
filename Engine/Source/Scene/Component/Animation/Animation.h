#pragma once
#include "Math/AssimpGLMHelper.h"
#include "Renderer/SkeletalMesh.h"
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <unordered_map>
#include <vector>


class SkeletalModel;
class Bone;
class Rig;

class Animation
{
public:
	Animation() = default;
	Animation(const aiNode* root, aiAnimation* animation, Ref<Rig> rig);
	~Animation() { }

	inline double GetTicksPerSecond() { return m_TicksPerSecond; }
	inline double GetDuration() { return m_Duration; }
	inline AssimpNodeData& GetRootNode() { return m_RootNode; }
	std::string GetAnimationName() { return m_Name; }

	uint32_t HowManyBones() const { return m_Bones.size(); }
	Ref<Bone> FindBone(std::string boneName);

	void DebugDisplayAnimationName()
	{
		std::cout << m_Name << "  dur: " << m_Duration << "  tps: " << m_TicksPerSecond << "\n";
	}

	void UpdateRig(aiAnimation* animation);


private:
	void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src, aiAnimation* animation);
	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string& NodeName);

	AssimpNodeData m_RootNode;
	std::string m_Name;
	double m_Duration;
	double m_TicksPerSecond;
	uint32_t m_NumChannels;
	Ref<Rig> m_Rig;
	std::vector<Ref<Bone>> m_Bones;  // internal -- send to Rig on animation change

	friend SkeletalMesh;
};
