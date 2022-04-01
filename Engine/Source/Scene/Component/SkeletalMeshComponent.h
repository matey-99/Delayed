#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer/SkeletalMesh.h"
#include "Material/Material.h"
#include "Material/ShaderLibrary.h"
#include "Scene/Component/RenderComponent.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

// q: How to convert aiMatrixXxX to glm::vecX format better
class AssimpGLMHelpers
{
public:

	static inline glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
	{
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}

	static inline glm::vec3 GetGLMVec(const aiVector3D& vec)
	{
		return glm::vec3(vec.x, vec.y, vec.z);
	}

	static inline glm::quat GetGLMQuat(const aiQuaternion& pOrientation)
	{
		return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
	}
};

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

struct Bone
{
public:
	glm::mat4 GetLocalTransform() { return m_LocalTransform; }
	std::string GetBoneName() const { return m_Name; }
	int GetBoneID() { return m_ID; }

	Bone(const std::string& name, int ID, const aiNodeAnim* channel);

	void Update(float animationTime);

	int GetPositionIndex(float animationTime);
	int GetRotationIndex(float animationTime);
	int GetScaleIndex(float animationTime);

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
};

class SkeletalMeshComponent : public RenderComponent
{
private:
	std::string m_Path;
	std::vector<SkeletalMesh> m_Meshes;
	std::vector<Ref<Material>> m_Materials;
	std::vector<std::string> m_MaterialsPaths;

	BoundingBox m_BoundingBox;

	bool m_MultipleMaterials;

public:
	SkeletalMeshComponent(Actor* owner);
	SkeletalMeshComponent(Actor* owner, std::string path);
	SkeletalMeshComponent(Actor* owner, std::string path, std::vector<std::string> materialsPath);

	void LoadMesh(std::string path);
	void LoadMaterial(std::string path);
	void ChangeMesh(std::string path);
	void ChangeMaterial(int index, std::string path);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void Destroy() override;

	inline std::string GetPath() const { return m_Path; }
	inline std::vector<SkeletalMesh> GetMeshes() const { return m_Meshes; }
	inline std::vector<Ref<Material>> GetMaterials() const { return m_Materials; }
	inline std::vector<std::string> GetMaterialsPaths() const { return m_MaterialsPaths; }
	inline BoundingBox GetBoundingBox() const { return m_BoundingBox; }
	uint32_t GetRenderedVerticesCount();

	inline void SetMaterial(int index, Ref<Material> material) { m_Materials.at(index) = material; }
};