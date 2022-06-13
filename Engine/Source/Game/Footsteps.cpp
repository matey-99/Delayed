#include "Footsteps.h"

#include "Scene/Scene.h"
#include "Scene/Actor.h"
#include "Game/Player.h"
#include "Game/CharacterController.h"
#include "Scene/Component/AudioSourceComponent.h"

Footsteps::Footsteps(Actor *owner) : GameComponent(owner) {

}

void Footsteps::Start() {
    m_AudioSource = m_Owner->GetComponent<AudioSourceComponent>();
    m_Player = m_Owner->GetScene()->GetComponent<Player>(m_PlayerID);
}

void Footsteps::Update(float deltaTime) {
    if (m_Player->GetCharacterController()->GetMovementSpeed() > 0.1f && m_Player->GetCharacterController()->IsGrounded()) {
        if (!m_AudioSource->IsPlaying())
            m_AudioSource->PlaySound();
    }
}
