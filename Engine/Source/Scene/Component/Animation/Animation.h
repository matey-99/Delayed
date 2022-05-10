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
	/*inline const std::unordered_map<std::string, BoneInfo>& GetBoneIDMap()
	{
		return m_BoneInfoMap;
	}*/
	std::string GetAnimationName() { return m_Name; }

	//Bone* FindBone(const std::string& name);

	void DebugDisplayAnimationNames()
	{
		std::cout << m_Name << "  dur: " << m_Duration << "  tps: " << m_TicksPerSecond << "\n";
	}

private:
	void ReadMissingBones(const aiAnimation* animation, Ref<Rig> rig);
	void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);

	AssimpNodeData m_RootNode;
	std::string m_Name;
	double m_Duration;
	double m_TicksPerSecond;
	uint32_t m_NumChannels;
	//std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;
	//std::vector<Bone> m_Bones;  // could be somehow abstracted

	friend SkeletalMesh;
};
