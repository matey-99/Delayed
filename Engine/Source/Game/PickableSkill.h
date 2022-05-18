#pragma once

#include "Core.h"

#include "GameComponent.h"
#include "Saveable.h"
#include "Scene/Component/Collider/ColliderComponent.h"

enum class SkillType
{
	DoubleJump, Dash, Teleport
};

class PickableSkill : public GameComponent, public Saveable
{
public:
	PickableSkill(Actor* owner);
	~PickableSkill();

	virtual void Start() override;

	virtual void OnTriggerEnter(ColliderComponent* other) override;

	virtual const SaveData Save() override;
	virtual void Load(const SaveData& data) override;

private:
	SkillType m_SkillType;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};