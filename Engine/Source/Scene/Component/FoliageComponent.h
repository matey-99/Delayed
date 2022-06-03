#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Assets/Model.h"
#include "Renderer/Mesh.h"
#include "Material/Material.h"
#include "Material/ShaderLibrary.h"
#include "Scene/Component/RenderComponent.h"

class FoliageComponent : public RenderComponent
{
public:
	FoliageComponent(Actor* owner);

	void LoadMaterial(std::string path);
	void ChangeMaterial(std::string path);
	void ChangeMaterial(Ref<Material> material);

	void LoadMesh(std::string path);
	void ChangeMesh(std::string path);
	void ChangeModel(Ref<Model> model);

	void SetInstancesCount(uint32_t instancesCount);
	std::vector<Ref<Mesh>> GetMeshes() const;
	uint32_t GetRenderedVerticesCount();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void PreRender() override;
	virtual void Render(Material::BlendMode blendMode) override;
	virtual void Destroy() override;

	inline Ref<Model> GetModel() const { return m_Model; }
	inline std::string GetPath() const { return m_Path; }
	inline Ref<Material> GetMaterial() const { return m_Material; }
	inline std::string GetMaterialPath() const { return m_MaterialPath; }
	inline uint32_t GetInstancesCount() const { return m_InstancesCount; }
	inline std::vector<glm::mat4> GetInstancesTransformations() const { return m_InstancesTransformations; }
	inline std::vector<glm::vec3> GetInstancesWorldPositions() const { return m_InstancesWorldPositions; }
	inline std::vector<BoundingBox> GetInstancesBoundingBoxes() const { return m_InstancesBoundingBoxes; }

	inline void SetMaterial(Ref<Material> material) { m_Material = material; }

private:
	void Generate();

private:
	Ref<Model> m_Model;
	std::string m_Path;
	Ref<Material> m_Material;
	std::string m_MaterialPath;

	uint64_t m_Seed;
	float m_Radius;
	uint32_t m_InstancesCount;
	std::vector<glm::mat4> m_InstancesTransformations;
	float m_MinInstanceScale;
	float m_MaxInstanceScale;

	std::vector<BoundingBox> m_InstancesBoundingBoxes;
	std::vector<glm::vec3> m_InstancesWorldPositions;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};