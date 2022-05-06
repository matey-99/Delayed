#include "Animation.h"
#include "Assets/SkeletalModel.h"


Animation::Animation(const aiScene* scene, int index, Ref<SkeletalModel> model)
	: m_Duration(0.f), m_TicksPerSecond(0)
{
	assert(scene && scene->mRootNode);
	if (scene->HasAnimations())
	{
		auto animation = scene->mAnimations[index];  // Here read animation properly
		m_Duration = animation->mDuration;
		m_TicksPerSecond = animation->mTicksPerSecond;
		aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
		globalTransformation = globalTransformation.Inverse();
		ReadHierarchyData(m_RootNode, scene->mRootNode);
		ReadMissingBones(animation, model);

		m_Name = animation->mName.C_Str();
	}
}

//Bone* Animation::FindBone(const std::string& name)
//{
//	auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
//		[&](const Bone& Bone)
//		{
//			return Bone.GetBoneName() == name;
//		}
//	);
//	if (iter == m_Bones.end()) return nullptr;
//	else return &(*iter);
//}

void Animation::ReadMissingBones(const aiAnimation* animation, Ref<SkeletalModel> model)
{
	//int size = animation->mNumChannels;
	//auto& boneInfoMap = model->GetBoneInfoMap(); //getting m_BoneInfoMap from Model class
	//uint32_t& boneCount = model->m_BoneCounter;  //getting the m_BoneCounter from Model class
	////reading channels(bones engaged in an animation and their keyframes)
	//for (int i = 0; i < size; i++)
	//{
	//	auto channel = animation->mChannels[i];
	//	std::string boneName = channel->mNodeName.data;
	//	if (boneInfoMap.find(boneName) == boneInfoMap.end())
	//	{
	//		boneInfoMap[boneName].ID = boneCount;
	//		boneCount++;
	//	}
	//	m_Bones.push_back(Bone(channel->mNodeName.data,
	//		boneInfoMap[channel->mNodeName.data].ID, channel));
	//}
	//m_BoneInfoMap = boneInfoMap;
}

void Animation::ReadHierarchyData(AssimpNodeData& dest, const aiNode* src)
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
