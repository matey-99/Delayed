#pragma once

#include "GameComponent.h"

class SceneTransition : public GameComponent {

public:

    SceneTransition(Actor *owner);

    void Start() override;

private:

    void LoadScene();

private:

    std::string m_Scene;
    TimerHandle m_TimerHandle;

    friend class SceneSerializer;

};
