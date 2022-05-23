#pragma once
#include "Core.h"
#include "Renderer/Bone.h"

struct BoneMap
{
	BoneMap(uint32_t id, std::string boneName, glm::mat4 offsetMatrix)
		:
		ID(id), Name(boneName), OffsetMatrix(offsetMatrix)
	{ }

	std::string Name;
	glm::mat4 OffsetMatrix;
	uint32_t ID;
};

class Rig
{
public:
	Rig() = default;
	//~Rig();


	bool AddBone(std::string boneName, glm::mat4 offsetMatrix);
	inline size_t HowManyBones() const { return m_BoneMapping.size(); }

	Ref<BoneMap> FindBone(std::string boneName) const;
	std::string GetName(uint32_t id) const;
	glm::mat4 GetOffsetMatrix(uint32_t id) const;
	glm::mat4 GetOffsetMatrix(std::string boneName) const;


private:
	std::vector<Ref<BoneMap>> m_BoneMapping;

};