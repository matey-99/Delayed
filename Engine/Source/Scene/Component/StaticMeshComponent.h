#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer/Mesh.h"
#include "Material/Material.h"
#include "Material/ShaderLibrary.h"
#include "Scene/Component/RenderComponent.h"

class StaticMeshComponent : public RenderComponent
{
private:
	std::string m_Path;
	std::vector<Mesh> m_Meshes;
	std::vector<Ref<Material>> m_Materials;
	std::vector<std::string> m_MaterialsPaths;

	BoundingBox m_BoundingBox;
    BoundingSphere m_BoundingSphere;

	bool m_MultipleMaterials;

public:
	StaticMeshComponent(Actor* owner);
	StaticMeshComponent(Actor* owner, std::string path);
	StaticMeshComponent(Actor* owner, std::string path, std::vector<std::string> materialsPath);

	void LoadMesh(std::string path);
	void LoadMaterial(std::string path);
	void ChangeMesh(std::string path);
	void ChangeMaterial(int index, std::string path);

	void UpdateBoundingBox();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void Destroy() override;

	inline std::string GetPath() const { return m_Path; }
	inline std::vector<Mesh> GetMeshes() const { return m_Meshes; }
	inline std::vector<Ref<Material>> GetMaterials() const { return m_Materials; }
	inline std::vector<std::string> GetMaterialsPaths() const { return m_MaterialsPaths; }
	inline BoundingBox GetBoundingBox() const { return m_BoundingBox; }
    inline BoundingSphere GetBoundingSphere() const { return m_BoundingSphere; }
    uint32_t GetRenderedVerticesCount();

	inline void SetMaterial(int index, Ref<Material> material) { m_Materials.at(index) = material; }
};