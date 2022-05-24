#include "AudioSourceComponent.h"
#include "Scene/Actor.h"

AudioSourceComponent::AudioSourceComponent(Actor *owner) : Component(owner) {
    m_Sound = "../../../Content/Audio/Music/music.mp3";
    m_Channel = CHANNEL_GROUP::MUSIC;
    m_Volume = 1.0f;
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
    if (m_3d)
        m_AudioSystem->SetChannel3dPosition(m_ChannelId, m_Owner->GetTransform()->GetWorldPosition());
}

void AudioSourceComponent::Destroy() {

}

void AudioSourceComponent::ChangeSound(std::string path) {
    m_Sound = path;
}

void AudioSourceComponent::PlaySound() {
    if (IsPlaying())
        StopSound();
    m_ChannelId = m_AudioSystem->PlaySound(m_Sound, m_Channel, m_Volume, m_Looping, m_3d,
                                           m_Owner->GetTransform()->GetWorldPosition());
}

void AudioSourceComponent::StopSound() {
    m_AudioSystem->StopChannel(m_ChannelId);
}

bool AudioSourceComponent::IsPlaying() {
    return m_AudioSystem->IsPlaying(m_ChannelId);
}

void AudioSourceComponent::SetVolume(float volume) {
    m_Volume = volume;
    m_AudioSystem->SetChannelVolume(m_ChannelId, m_Volume);
}

void AudioSourceComponent::UpdateMode() {
    m_AudioSystem->SetChannelMode(m_ChannelId, m_3d, m_Looping);
}