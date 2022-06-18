#include "SpaceshipPart.h"

#include "Scene/Actor.h"
#include "Player.h"
#include "SaveManager.h"
#include "Scene/Scene.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Assets/AssetManager.h"

SpaceshipPart::SpaceshipPart(Actor* owner)
	: GameComponent(owner)
{
	m_Type = SpaceshipPartType::Part1;
	m_Material = AssetManager::LoadMaterial("Materials/Default.mat");
	m_Placed = false;
}

SpaceshipPart::~SpaceshipPart()
{
}

void SpaceshipPart::Start()
{
	if (auto mesh = m_Owner->GetComponent<StaticMeshComponent>())
	{
		mesh->ChangeMaterial(0, "Materials/Hologram.mat");
	}
}

const SaveData SpaceshipPart::Save()
{
	SaveData data;
	data.ActorID = m_Owner->GetID();
	data.BoolFields.insert({ "Placed", m_Placed });

	return data;
}

void SpaceshipPart::Load(const SaveData& data)
{
	bool placed = data.BoolFields.find("Placed")->second;

	if (placed)
		Fix();
	else
	{
		if (auto mesh = m_Owner->GetComponent<StaticMeshComponent>())
		{
			mesh->ChangeMaterial(0, "Materials/Hologram.mat");
			m_Placed = false;
		}
	}
}

void SpaceshipPart::Fix()
{
	if (auto mesh = m_Owner->GetComponent<StaticMeshComponent>())
	{
		mesh->ChangeMaterial(0, m_Material);
		m_Placed = true;
	}
}
