#pragma once
#include "Math/AssimpGLMHelper.h"
#include "Renderer/SkeletalMesh.h"
//#include "Renderer/Bone.h"
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <unordered_map>
#include <vector>


class SkeletalModel;
class Bone;

class Animation
{
public:
	Animation() = default;
	Animation(const aiScene* scene, int index, Ref<SkeletalModel> model);
	~Animation() { }

	inline float GetTicksPerSecond() { return m_TicksPerSecond; }
	inline float GetDuration() { return m_Duration; }
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
	void ReadMissingBones(const aiAnimation* animation, Ref<SkeletalModel> model);
	void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);

	AssimpNodeData m_RootNode;
	//std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;
	//std::vector<Bone> m_Bones;  // could be somehow abstracted
	std::string m_Name;
	float m_Duration;
	int m_TicksPerSecond;

	friend SkeletalMesh;
};
