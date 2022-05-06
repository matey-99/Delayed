#include "Rig.h"


void Rig::AddBone(Ref<Bone> bone)
{
	if (FindBone(bone->GetBoneName()) == nullptr)
	{
		m_Bones.push_back(bone);
	}
}

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
