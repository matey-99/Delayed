#include "Mushroom.h"

#include "Scene/Actor.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Material/MaterialInstance.h"
#include "Player.h"

int Mushroom::m_GlobalActivePath;

Mushroom::Mushroom(Actor *owner) : GameComponent(owner) {
    m_Path = 0;
    m_Material = nullptr;
    m_Enabled = true;
}

void Mushroom::Start() {
    m_StaticMesh = m_Owner->GetComponent<StaticMeshComponent>();
    m_Material = MaterialInstance::Create(m_StaticMesh->GetMaterials()[0]);
    m_StaticMesh->SetMaterial(0, m_Material);

    if (m_Enabled && m_Path == m_GlobalActivePath) {
        SetOn();
    }
}

void Mushroom::OnTriggerEnter(ColliderComponent *other) {
    if (other->GetOwner()->GetComponent<Player>()) {
        m_Enabled = false;
        SetOff();
    }

}

const SaveData Mushroom::Save() {
    SaveData data;
    data.ActorID = m_Owner->GetID();
    data.BoolFields.insert({"Enabled", m_Enabled});

    return data;
}

void Mushroom::Load(const SaveData &data) {
    m_Enabled = data.BoolFields.find("Enabled")->second;

    if ((!m_Enabled || m_Path != m_GlobalActivePath) && m_Material) {
        SetOff();
    } else if (m_Material) {
        SetOn();
    }
}

void Mushroom::SetOff() {
    m_Material->SetFloatParameter("u_Material.emissiveStrength", 0.1f);
}

void Mushroom::SetOn() {
    m_Material->SetFloatParameter("u_Material.emissiveStrength", 1.0f);
}

