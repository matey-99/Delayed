#include "Footsteps.h"

#include "Scene/Scene.h"
#include "Scene/Actor.h"
#include "Game/Player.h"
#include "Game/TPPPlayer.h"
#include "Game/CharacterController.h"
#include "Scene/Component/AudioSourceComponent.h"
#include "Math/Math.h"

Footsteps::Footsteps(Actor *owner) : GameComponent(owner) {
    m_Sounds = std::vector<std::string>();
}

void Footsteps::Start() {
    m_AudioSource = m_Owner->GetComponent<AudioSourceComponent>();
    m_CharachterController = m_Owner->GetScene()->GetComponent<CharacterController>(m_PlayerID);
    m_Player = m_Owner->GetScene()->GetComponent<Player>(m_PlayerID);
}

void Footsteps::Update(float deltaTime) {

    if (m_CharachterController->IsJumping()) {
        m_AudioSource->ChangeSound(m_Sounds.at(10));
        m_AudioSource->PlaySound();
    }

    if (m_CharachterController->GetMovementSpeed() > 0.1f && m_CharachterController->IsGrounded()) {
        if (!m_AudioSource->IsPlaying()) {
            int sound;
            if (m_Player->IsRunning())
                sound = Math::RandomRange(5, 9);
            else
                sound = Math::RandomRange(0, 4);

            m_AudioSource->ChangeSound(m_Sounds.at(sound));
            m_AudioSource->PlaySound();
        }
    }
}
