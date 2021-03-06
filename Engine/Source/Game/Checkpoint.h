#pragma once

#include "Core.h"

#include "GameComponent.h"
#include "Saveable.h"
#include "Scene/Component/Collider/ColliderComponent.h"

class MaterialInstance;

class Checkpoint : public GameComponent, public Saveable
{
public:
	Checkpoint(Actor* owner);
	~Checkpoint();

	virtual void Start() override;

	virtual void OnTriggerEnter(ColliderComponent* other) override;

	virtual const SaveData Save() override;
	virtual void Load(const SaveData& data) override;

private:
    void SetMaterial();

private:
    bool m_Enabled;

    Ref<MaterialInstance> m_Material;
    Ref<Texture> m_BaseTexture;
    Ref<Texture> m_EmissiveTexture;


#pragma region Serialization

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};