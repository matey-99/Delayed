#pragma once

#include "Component.h"

#include "MeshComponent.h"
#include "Assets/ModelBase.h"
#include "Material/Material.h"

struct LOD
{
	uint16_t Level;
	Ref<ModelBase> Model;
	std::vector<Ref<Material>> Materials;
	float MaxDistance;

	bool operator !=(const LOD& other)
	{
		return Level != other.Level;
	}
};

class LODGroupComponent : public Component
{
public:
	LODGroupComponent(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

	inline LOD GetCurrentLOD() const { return m_CurrentLOD; }
	inline const std::vector<LOD>& GetLODs() const { return m_LODs; }

private:
	LOD m_CurrentLOD;
	std::vector<LOD> m_LODs;

	Ref<MeshComponent> m_MeshComponent;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};