#include "AudioSourceComponent.h"
#include "Scene/Actor.h"

AudioSourceComponent::AudioSourceComponent(Actor *owner) : Component(owner) {
    m_Sound = "../../../Content/Audio/Music/music.mp3";
    m_Volume = 0.25f;
    m_3d = false;
    m_Looping = false;
    m_PlayOnStart = false;
    m_ChannelId = -1;
}

void AudioSourceComponent::Start() {
    m_AudioSystem = AudioSystem::GetInstance();
    if (m_PlayOnStart)
        PlaySound();
}

void AudioSourceComponent::Update(float deltaTime) {
    m_AudioSystem->SetChannel3dPosition(m_ChannelId, m_Owner->GetTransform()->GetWorldPosition());
}

void AudioSourceComponent::Destroy() {

}

void AudioSourceComponent::ChangeSound(std::string path) {
    m_Sound = path;
}

void AudioSourceComponent::PlaySound() {
    if (m_AudioSystem->IsPlaying(m_ChannelId))
        return;
    m_ChannelId = m_AudioSystem->PlaySound(m_Sound, m_Volume, m_Looping, m_3d, m_Owner->GetTransform()->GetWorldPosition());
}

void AudioSourceComponent::StopSound() {
    if (m_AudioSystem->IsPlaying(m_ChannelId))
        m_AudioSystem->StopChannel(m_ChannelId);
}