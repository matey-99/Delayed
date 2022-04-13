#pragma once

#include "Renderer/SkeletalMesh.h"
#include "Material/Material.h"
#include "Material/ShaderLibrary.h"
#include "Scene/Component/RenderComponent.h"

class SkeletalMeshComponent : public RenderComponent
{
private:
	std::string m_Path;
	std::vector<SkeletalMesh> m_Meshes;
	std::vector<Ref<Material>> m_Materials;
	std::vector<std::string> m_MaterialsPaths;

	BoundingBox m_BoundingBox;
	BoundingSphere m_BoundingSphere;

<<<<<<< Updated upstream
	bool m_MultipleMaterials;

=======
>>>>>>> Stashed changes
public:
	SkeletalMeshComponent(Actor* owner);
	SkeletalMeshComponent(Actor* owner, std::string path);
	SkeletalMeshComponent(Actor* owner, std::string path, std::vector<std::string> materialsPath);

	void LoadMesh(std::string path);
	void LoadMaterial(std::string path);
	void ChangeMesh(std::string path);
	void ChangeMaterial(int index, std::string path);

	void UpdateBoundingBox();
	void UpdateBoundingSphere();

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
	inline BoundingSphere GetBoundingSphere() const { return m_BoundingSphere; }
	uint32_t GetRenderedVerticesCount();

	// Details
	uint32_t GetBoneCount();


	inline void SetMaterial(int index, Ref<Material> material) { m_Materials.at(index) = material; }
};