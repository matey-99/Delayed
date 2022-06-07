#include <Scene/Component/StaticMeshComponent.h>
#include <Material/MaterialInstance.h>
#include "Checkpoint.h"

#include "Scene/Actor.h"
#include "Player.h"
#include "SaveManager.h"
#include "Assets/AssetManager.h"

Checkpoint::Checkpoint(Actor* owner)
	: GameComponent(owner)
{
    m_Enabled = false;
    m_Material = nullptr;

    m_BaseTexture = AssetManager::LoadTexture("Textures/Checkpoint_tex/checkpoint_blue_BaseColor.png", Texture::Type::BaseColor);
    m_EmissiveTexture = AssetManager::LoadTexture("Textures/Checkpoint_tex/checkpoint_blue_Emissive.png", Texture::Type::Emissive);
}

Checkpoint::~Checkpoint()
{
}

void Checkpoint::Start()
{
	if (auto collider = m_Owner->GetComponent<ColliderComponent>())
	{
		collider->OnTriggerEnterDelegate.Add(&Checkpoint::OnTriggerEnter, this);
	}

    if (auto mesh = m_Owner->GetComponent<StaticMeshComponent>())
    {
        Ref<Material> material = mesh->GetMaterials()[0];
        m_Material = MaterialInstance::Create(material);

        mesh->SetMaterial(0, m_Material);
    }

}

void Checkpoint::OnTriggerEnter(ColliderComponent* other)
{
    if (m_Enabled)
        return;

	if (auto player = other->GetOwner()->GetComponent<Player>())
	{
        if (!m_Owner->GetTransform()->GetChildren().empty()) {
            player->SetLastCheckpoint(m_Owner->GetTransform()->GetChildren()[0]->GetWorldPosition());
        } else {
            player->SetLastCheckpoint(m_Owner->GetTransform()->GetWorldPosition());
        }

		m_Enabled = true;
        SetMaterial();

		SaveManager::GetInstance()->SaveGame();
	}
}

const SaveData Checkpoint::Save()
{
	SaveData data;
	data.ActorID = m_Owner->GetID();
	data.BoolFields.insert({ "Enabled", m_Enabled });

	return data;
}

void Checkpoint::Load(const SaveData& data)
{
	bool enabled = data.BoolFields.find("Enabled")->second;

	m_Enabled = enabled;

    if (m_Enabled) {
        SetMaterial();
    }
}

void Checkpoint::SetMaterial() {
    m_Material->SetTexture2DParameter("u_Material.albedoMap", m_BaseTexture);
    m_Material->SetTexture2DParameter("u_Material.emissiveMap", m_EmissiveTexture);
    m_Material->SetFloatParameter("u_Material.emissiveStrength", 2.0f);
}
