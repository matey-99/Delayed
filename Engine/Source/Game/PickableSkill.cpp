#include "PickableSkill.h"

#include "Scene/Actor.h"
#include "Player.h"
#include "SaveManager.h"
#include "Scene/Scene.h"
#include "NotificationManager.h"
#include "TutorialManager.h"

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

		std::string skillName = "";
		TutorialType tutorial = TutorialType::DoubleJump;
		switch (m_SkillType)
		{
		case SkillType::DoubleJump:
			skillName = "Double Jump";
			tutorial = TutorialType::DoubleJump;
			break;
		case SkillType::Dash:
			skillName = "Dash";
			tutorial = TutorialType::Dash;
			break;
		case SkillType::Teleport:
			skillName = "Teleportation to the Ghost";
			tutorial = TutorialType::Teleport;
			break;
		}

		NotificationManager::GetInstance()->Notify("You obtained a new skill: " + skillName);
		TutorialManager::GetInstance()->DisplayTutorial(tutorial);
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
