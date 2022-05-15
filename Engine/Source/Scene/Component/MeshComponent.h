#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Assets/ModelBase.h"
#include "Renderer/Mesh.h"
#include "Material/Material.h"
#include "Material/ShaderLibrary.h"
#include "Scene/Component/RenderComponent.h"

class MeshComponent : public RenderComponent
{
public:
	MeshComponent(Actor* owner);
	MeshComponent(Actor* owner, std::string path);
	MeshComponent(Actor* owner, std::string path, std::vector<std::string> materialsPath);

	void LoadMaterial(std::string path);
	void ChangeMaterial(int index, std::string path);
	void ChangeMaterial(int index, Ref<Material> material);
	void ChangeMaterials(std::vector<Ref<Material>> materials);

	virtual void LoadMesh(std::string path) = 0;
	virtual void ChangeMesh(std::string path) = 0;
	virtual void ChangeModel(Ref<ModelBase> model) = 0;

	virtual void UpdateBoundingBox() = 0;
    virtual void UpdateBoundingSphere() = 0;

	virtual Ref<ModelBase> GetModel() const = 0;
	virtual std::vector<Ref<MeshBase>> GetMeshes() const = 0;
	virtual uint32_t GetRenderedVerticesCount() = 0;

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void PreRender() override;
	virtual void Render(Material::BlendMode blendMode) override;
	virtual void Destroy() override;

	inline std::string GetPath() const { return m_Path; }
	inline std::vector<Ref<Material>> GetMaterials() const { return m_Materials; }
	inline std::vector<std::string> GetMaterialsPaths() const { return m_MaterialsPaths; }
	inline BoundingBox GetBoundingBox() const { return m_BoundingBox; }
    inline BoundingSphere GetBoundingSphere() const { return m_BoundingSphere; }

	inline void SetMaterial(int index, Ref<Material> material) { m_Materials.at(index) = material; }

protected:
	std::string m_Path;
	std::vector<Ref<Material>> m_Materials;
	std::vector<std::string> m_MaterialsPaths;

	BoundingBox m_BoundingBox;
	BoundingSphere m_BoundingSphere;

	bool m_MultipleMaterials;
};