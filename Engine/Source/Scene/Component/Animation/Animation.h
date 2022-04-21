#pragma once
#include "Math/AssimpGLMHelper.h"
#include "Renderer/SkeletalMesh.h"
#include "Renderer/Bone.h"
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <unordered_map>
#include <vector>

class Animation
{
public:
	Animation() = default;

	Animation(const aiScene* scene, Ref<SkeletalMesh> model)
	{
		assert(scene && scene->mRootNode);
		if (scene->HasAnimations())
		{
			auto animation = scene->mAnimations[0];  // Here read animation properly
			m_Duration = animation->mDuration;
			m_TicksPerSecond = animation->mTicksPerSecond;
			aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
			globalTransformation = globalTransformation.Inverse();
			ReadHierarchyData(m_RootNode, scene->mRootNode);
			ReadMissingBones(animation, model);
		}
		
		// Set animation names to vector
		for (int i = 0; i < scene->mNumAnimations; i++)
		{
			auto animation = scene->mAnimations[i];

			m_Names.push_back(animation->mName.C_Str());
		}
		
	}

	~Animation() { }

	Bone* FindBone(const std::string& name)
	{
		auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
			[&](const Bone& Bone)
			{
				return Bone.GetBoneName() == name;
			}
		);
		if (iter == m_Bones.end()) return nullptr;
		else return &(*iter);
	}

	inline float GetTicksPerSecond() { return m_TicksPerSecond; }
	inline float GetDuration() { return m_Duration; }
	inline AssimpNodeData& GetRootNode() { return m_RootNode; }
	inline const std::unordered_map<std::string, BoneInfo>& GetBoneIDMap()
	{
		return m_BoneInfoMap;
	}

	void DebugDisplayAnimationNames()
	{
		for (auto& name : m_Names)
			std::cout << name << "\n";
	}

private:
	void ReadMissingBones(const aiAnimation* animation, Ref<SkeletalMesh> model)
	{
		int size = animation->mNumChannels;

		auto& boneInfoMap = model->GetBoneInfoMap(); //getting m_BoneInfoMap from Model class
		uint32_t& boneCount = model->m_BoneCounter;  //getting the m_BoneCounter from Model class

		//reading channels(bones engaged in an animation and their keyframes)
		for (int i = 0; i < size; i++)
		{
			auto channel = animation->mChannels[i];
			std::string boneName = channel->mNodeName.data;

			if (boneInfoMap.find(boneName) == boneInfoMap.end())
			{
				boneInfoMap[boneName].ID = boneCount;
				boneCount++;
			}
			m_Bones.push_back(Bone(channel->mNodeName.data,
				boneInfoMap[channel->mNodeName.data].ID, channel));
		}

		m_BoneInfoMap = boneInfoMap;
	}


	void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src)
	{
		assert(src);

		dest.name = src->mName.data;
		dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
		dest.childrenCount = src->mNumChildren;

		for (int i = 0; i < src->mNumChildren; i++)
		{
			AssimpNodeData newData;
			ReadHierarchyData(newData, src->mChildren[i]);
			dest.children.push_back(newData);
		}
	}

	AssimpNodeData m_RootNode;
	std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;
	std::vector<Bone> m_Bones;  // could be somehow abstracted
	std::vector<std::string> m_Names;
	float m_Duration;
	int m_TicksPerSecond;

	friend SkeletalMesh;
};
