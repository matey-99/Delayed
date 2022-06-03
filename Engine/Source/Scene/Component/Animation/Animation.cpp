#include "Animation.h"
#include "Assets/SkeletalModel.h"


//Animation::Animation(const aiNode* root, int index, Ref<SkeletalModel> model)
Animation::Animation(const aiNode* root, aiAnimation* animation, Ref<Rig> rig)
{
	//m_AiAnimation = animation;

	m_Rig = rig;
	m_Name = animation->mName.C_Str();
	m_Duration = animation->mDuration;
	m_NumChannels = animation->mNumChannels;
	m_TicksPerSecond = animation->mTicksPerSecond;

	ReadHierarchyData(m_RootNode, root);
	UpdateRig(animation);

	// Code below does nothing but was originally here -- maybe should inverse a reference? idk
	//aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;  // is necessery?
	//globalTransformation = globalTransformation.Inverse();

}

Ref<Bone> Animation::FindBone(std::string boneName)
{
	for (auto& bone : m_Bones)
	{
		if (bone->GetBoneName() == boneName)
			return bone;
	}

	return nullptr;
}

//void Animation::ReadMissingBones(const aiAnimation* animation, Ref<SkeletalModel> model)
void Animation::UpdateRig(aiAnimation* animation)
{
	// New, with BoneMapping:
	for (int index = 0; index < m_NumChannels; index++)
	{
		auto channel = animation->mChannels[index];
		std::string boneName = channel->mNodeName.data;

		Ref<BoneMap> boneMap = m_Rig->FindBone(boneName);

		if (boneMap == nullptr)  // Not found such bone
		{
			//std::cout << "Animation.cpp: Found missing bone with name: " << boneName << '\n';  // Debug
			// Here add the missing bone
			/*Ref<Bone> bone = CreateRef<Bone>(boneName, );
			m_Bones.push_back(bone);*/
		}
		else  // Found bone
		{
			//std::cout << m_Name << " (Animation.cpp) found, added and completed bone with name: " << boneName << "\n";  // Debug

			Ref<Bone> bone = CreateRef<Bone>(boneMap->Name, boneMap->ID, boneMap->OffsetMatrix);
			m_Bones.push_back(bone);
			m_Bones.back()->ReadDataFromAnimation(channel);
		}
	}



	// Old:
	//uint32_t boneCount = m_Rig->HowManyBones();

	//for (int i = 0; i < m_NumChannels; i++)
	//{
	//	auto channel = animation->mChannels[i];
	//	std::string boneName = channel->mNodeName.data;

	//	if (m_Rig->FindBone(boneName) == nullptr)
	//	{
	//		std::cout << "Animation.cpp: Found missing bone with name: " << boneName << '\n';  // Debug
	//		// Here add the missing bone
	//		//m_Rig->AddBone(CreateRef<Bone>());
	//		//boneCount = m_Rig->HowManyBones();
	//	}
	//	else
	//	{
	//		std::cout << "Animation.cpp: Completed bone with name: " << boneName << "\n";  // Debug
	//		m_Rig->CompleteBone(boneName, channel);
	//	}
	//}


	//// old:
	////int size = animation->mNumChannels; // use m_NumChannels instead
	//auto& boneInfoMap = model->GetBoneInfoMap(); //getting m_BoneInfoMap from Model class
	//uint32_t& boneCount = model->m_BoneCounter;  //getting the m_BoneCounter from Model class
	////reading channels(bones engaged in an animation and their keyframes)
	//for (int i = 0; i < m_NumChannels; i++)
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

	//std::cout << "AssimpNodeData mName: " << dest.name << "\n";

	for (int i = 0; i < src->mNumChildren; i++)
	{
		// There we should traverse bones looking for some transformation from animation (1:37:00)
		//for (int index = 0; index < animation->mNumChannels; index++)
		//{
		//	const aiNodeAnim* nodeAnim = animation->mChannels[index];
		//	if (std::string(nodeAnim->mNodeName.data) == src->mName.C_Str())
		//	{
		//		// Calculate transformation matrix
		//		this->
		//	}
		//}

		AssimpNodeData newData;
		ReadHierarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}