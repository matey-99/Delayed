#include "Rig.h"


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
