#include "PickableSkill.h"

#include "Scene/Actor.h"
#include "Player.h"
#include "SaveManager.h"
#include "Scene/Scene.h"

PickableSkill::PickableSkill(Actor* owner)
	: GameComponent(owner)
{
	m_SkillType = SkillType::DoubleJump;
}

PickableSkill::~PickableSkill()
{
}

void PickableSkill::Start()
{
	if (auto collider = m_Owner->GetComponent<ColliderComponent>())
	{
		collider->OnTriggerEnterDelegate.Add(&PickableSkill::OnTriggerEnter, this);
	}
}

void PickableSkill::OnTriggerEnter(ColliderComponent* other)
{
	if (auto player = other->GetOwner()->GetComponent<Player>())
	{
		player->AddSkill(m_SkillType);
		m_Owner->SetEnabled(false);
	}
}

const SaveData PickableSkill::Save()
{
	SaveData data;
	data.ActorID = m_Owner->GetID();
	data.BoolFields.insert({ "Enabled", m_Owner->IsEnabled() });

	return data;
}

void PickableSkill::Load(const SaveData& data)
{
	bool enabled = data.BoolFields.find("Enabled")->second;

	m_Owner->SetEnabled(enabled);
}
