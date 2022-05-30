#pragma once

#include "Core.h"

struct SaveData
{
	uint64_t ActorID;
	std::unordered_map<std::string, bool> BoolFields;
	std::unordered_map<std::string, int> IntFields;
	std::unordered_map<std::string, float> FloatFields;
	std::unordered_map<std::string, std::string> StringFields;
	std::unordered_map<std::string, glm::vec3> Vector3Fields;
};

class Saveable
{
public:
	virtual const SaveData Save() = 0;
	virtual void Load(const SaveData& data) = 0;
};