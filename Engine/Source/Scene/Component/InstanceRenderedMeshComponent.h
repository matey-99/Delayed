#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer/Mesh.h"
#include "Material/Material.h"
#include "Material/ShaderLibrary.h"
#include "Scene/Component/RenderComponent.h"

class InstanceRenderedMeshComponent : public RenderComponent
{
private:
	std::string m_Path;
	std::vector<Mesh> m_Meshes;
	std::vector<Ref<Material>> m_Materials;
	std::vector<std::string> m_MaterialsPaths;

	bool m_MultipleMaterials;

	int32_t m_InstancesCount;
	float m_Radius;
	float m_MinMeshScale;
	float m_MaxMeshScale;

	std::vector<glm::mat4> m_ModelMatrices;

	uint32_t m_ModelMatricesBuffer;

public:
	InstanceRenderedMeshComponent(Entity* owner);
	InstanceRenderedMeshComponent(Entity* owner, std::string path);
	InstanceRenderedMeshComponent(Entity* owner, std::string path, std::vector<std::string> materialsPath);

	void LoadMesh(std::string path);
	void LoadMaterial(std::string path);
	void ChangeMesh(std::string path);
	void ChangeMaterial(int index, std::string path);

	void Generate();

	virtual void Begin() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void Destroy() override;

	inline std::string GetPath() const { return m_Path; }
	inline std::vector<Mesh> GetMeshes() const { return m_Meshes; }
	inline std::vector<Ref<Material>> GetMaterials() const { return m_Materials; }
	inline std::vector<std::string> GetMaterialsPaths() const { return m_MaterialsPaths; }
	inline int32_t GetInstancesCount() const { return m_InstancesCount; }
	inline float GetRadius() const { return m_Radius; }
	inline float GetMinMeshScale() const { return m_MinMeshScale; }
	inline float GetMaxMeshScale() const { return m_MaxMeshScale; }
	inline std::vector<glm::mat4> GetModelMatrices() const { return m_ModelMatrices; }
	inline uint32_t GetModelMatricesBuffer() const { return m_ModelMatricesBuffer; }
	uint32_t GetRenderedVerticesCount();

	inline void SetMaterial(int index, Ref<Material> material) { m_Materials.at(index) = material; }
	inline void SetInstancesCount(int32_t count) { m_InstancesCount = count; }
	inline void SetRadius(float radius) { m_Radius = radius; }
	inline void SetMinMeshScale(float minMeshScale) { m_MinMeshScale = minMeshScale; }
	inline void SetMaxMeshScale(float maxMeshScale) { m_MaxMeshScale = maxMeshScale; }

	friend class EntityDetailsPanel;
	friend class SceneSerializer;
};