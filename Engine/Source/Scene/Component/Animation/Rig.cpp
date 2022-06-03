#include "Rig.h"


//void Rig::AddBone(Ref<Bone> bone)
//{
//	if (FindBone(bone->GetBoneName()) == nullptr)
//	{
//		m_Bones.push_back(bone);
//	}
//}

//void Rig::CompleteBone(std::string boneName, aiNodeAnim* channel)
//{
//	auto bone = FindBone(boneName);
//	if (bone != nullptr)
//		bone->ReadDataFromAnimation(channel);
//
//}

/*
Ref<Bone> Rig::GetBone(uint32_t index) const
{
	if (index < HowManyBones() && index >= 0)
		return m_Bones[index];

	assert(0);  // Attempt to get bone with index that does not exist
}
Ref<Bone> Rig::FindBone(std::string name) const
{
	for (auto& bone : m_Bones)
	{
		if (bone->GetBoneName() == name)
			return bone;
	}

	return nullptr;
}
Ref<Rig> Rig::Clone()
{
	Ref<Rig> rig;

	// Deep copy:
	for (int index = 0; index < HowManyBones(); index++)
	{
		Ref<Bone> bone = CreateRef<Bone>(m_Bones[index]->GetBoneName(), HowManyBones(), m_Bones[index]->GetOffset());
		rig->AddBone(bone);
	}

	return rig;
}
*/

bool Rig::AddBone(std::string boneName, glm::mat4 offsetMatrix)
{
	if (FindBone(boneName) == nullptr)
	{
		m_BoneMapping.push_back(CreateRef<BoneMap>(HowManyBones(), boneName, offsetMatrix));
		
		return true;
	}

	return false;
}

Ref<BoneMap> Rig::FindBone(std::string boneName) const
{
	for (auto& bone : m_BoneMapping)
	{
		if (bone->Name == boneName)
			return bone;
	}

	return nullptr;
}

std::string Rig::GetName(uint32_t id) const
{
	if (id >= 0 && id < HowManyBones())
		return m_BoneMapping[id]->Name;

	return "";
}

glm::mat4 Rig::GetOffsetMatrix(uint32_t id) const
{
	if (id >= 0 && id < HowManyBones())
		return m_BoneMapping[id]->OffsetMatrix;
}

glm::mat4 Rig::GetOffsetMatrix(std::string boneName) const
{
	Ref<BoneMap> boneMap = FindBone(boneName);

	if (boneMap->ID >= 0)
	{
		return m_BoneMapping[boneMap->ID]->OffsetMatrix;
	}
	
	return glm::mat4(-1.0);  // not found
}
