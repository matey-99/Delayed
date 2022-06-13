#pragma once

#include "GameComponent.h"

class AudioSourceComponent;
class Player;

class Footsteps : public GameComponent {

public:
    Footsteps(Actor *owner);

    void Start() override;

    void Update(float deltaTime) override;

private:
    Ref<AudioSourceComponent> m_AudioSource;
    Ref<Player> m_Player;
    std::vector<std::string> m_Sounds;

#pragma region Serialization

    uint64_t m_PlayerID;

#pragma endregion

    friend class SceneSerializer;
};
