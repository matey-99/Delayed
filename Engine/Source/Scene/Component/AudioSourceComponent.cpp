#include "AudioSourceComponent.h"
#include "Scene/Actor.h"

AudioSourceComponent::AudioSourceComponent(Actor *owner) : Component(owner) {}

void AudioSourceComponent::Start() {
    m_AudioSystem = AudioSystem::GetInstance();
}

void AudioSourceComponent::Update(float deltaTime) {
    m_AudioSystem->SetChannel3dPosition(m_ChannelId, m_Owner->GetTransform()->GetWorldPosition());
}

void AudioSourceComponent::Destroy() {

}

void AudioSourceComponent::SetSound(const std::string& sound) {
    m_Sound = "../../../Content/Audio/" + sound;
}

void AudioSourceComponent::SetVolume(float volume) {
    m_Volume = volume;
}

void AudioSourceComponent::Set3d(bool is3d) {
    m_3d = is3d;
}

void AudioSourceComponent::SetLooping(bool isLooping) {
    m_Looping = isLooping;
}

void AudioSourceComponent::PlaySound() {
    m_ChannelId = m_AudioSystem->PlaySound(m_Sound, m_Volume, m_Looping, m_3d, m_Owner->GetTransform()->GetWorldPosition());
}

