#include "AudioListenerComponent.h"
#include "Scene/Actor.h"


AudioListenerComponent::AudioListenerComponent(Actor *owner) : Component(owner) {}

void AudioListenerComponent::Start() {
    m_AudioSystem = AudioSystem::GetInstance();
}

void AudioListenerComponent::Update(float deltaTime) {
    m_AudioSystem->Set3dListener(m_Owner->GetTransform()->GetWorldPosition(), m_Owner->GetTransform()->GetForward(), glm::cross(m_Owner->GetTransform()->GetRight(), m_Owner->GetTransform()->GetForward()));
}

void AudioListenerComponent::Destroy() {

}
