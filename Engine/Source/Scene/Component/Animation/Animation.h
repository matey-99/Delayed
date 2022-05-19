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
	Animation(const aiNode* root, const aiAnimation* animation, Ref<Rig> rig);
	~Animation() { }

	inline double GetTicksPerSecond() { return m_TicksPerSecond; }
	inline double GetDuration() { return m_Duration; }
	inline AssimpNodeData& GetRootNode() { return m_RootNode; }
	std::string GetAnimationName() { return m_Name; }

	void DebugDisplayAnimationName()
	{
		std::cout << m_Name << "  dur: " << m_Duration << "  tps: " << m_TicksPerSecond << "\n";
	}

	void ReadMissingBones(const aiAnimation* animation);


private:
	void ReadHierarchyData(const aiAnimation* animation, AssimpNodeData& dest, const aiNode* src);

	AssimpNodeData m_RootNode;
	std::string m_Name;
	double m_Duration;
	double m_TicksPerSecond;
	uint32_t m_NumChannels;
	Ref<Rig> m_Rig;
	//std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;
	//std::vector<Bone> m_Bones;  // could be somehow abstracted

	friend SkeletalMesh;
};