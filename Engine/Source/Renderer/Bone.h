#pragma once
#include "Core.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation
{
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

// Needs to be complete in order to work properly (bool isComplete must be 1)
class Bone
{
public:
	glm::mat4 GetLocalTransform() { return m_LocalTransform; }
	std::string GetBoneName() const { return m_Name; }
	int GetID() { return m_ID; }
	glm::mat4 GetOffset() { return m_OffsetMatrix; }
	bool IsComplete() { return isComplete; }

	//Bone(const std::string& name, int ID, const aiNodeAnim* channel);
	Bone(const std::string& name, int ID, glm::mat4 offsetMatrix);  // needs to get info from animNode for every animation
	Bone() = default;

	void Update(float animationTime);

	int GetPositionIndex(float animationTime);
	int GetRotationIndex(float animationTime);
	int GetScaleIndex(float animationTime);

	void ReadDataFromAnimation(const aiNodeAnim* channel);

private:
	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

	glm::mat4 InterpolatePosition(float animationTime);
	glm::mat4 InterpolateRotation(float animationTime);
	glm::mat4 InterpolateScaling(float animationTime);

	std::vector<KeyPosition> m_Positions;
	std::vector<KeyRotation> m_Rotations;
	std::vector<KeyScale> m_Scales;
	int m_NumPositions;
	int m_NumRotations;
	int m_NumScalings;

	glm::mat4 m_LocalTransform;
	std::string m_Name;

	int m_ID;
	glm::mat4 m_OffsetMatrix;  // transforms vertex: model space -> bone space

	// Bone is complete when it gets information from aiNodeAnim*.
	// If you used constructor without aiNodeAnim* argument this is probably why it's zero
	bool isComplete;
};
