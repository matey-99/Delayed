#include "AudioSourceComponent.h"

AudioSourceComponent::AudioSourceComponent(Actor *owner) : Component(owner) {}

void AudioSourceComponent::Start() {

}

void AudioSourceComponent::Update(float deltaTime) {

}

void AudioSourceComponent::Destroy() {

}

void AudioSourceComponent::SetSound(const std::string& sound) {
    m_SoundName = "../../../Content/Audio/" + sound;
}
