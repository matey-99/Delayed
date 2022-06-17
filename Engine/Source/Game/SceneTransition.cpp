#include "SceneTransition.h"

#include "Scene/SceneManager.h"
#include "Time/TimerManager.h"
#include "Audio/AudioSystem.h"

SceneTransition::SceneTransition(Actor *owner) : GameComponent(owner) {
    m_Scene = "Scenes/Prototype.scene";
}

void SceneTransition::Start() {
    Event e;
    e.Add(&SceneTransition::LoadScene, this);

    m_TimerHandle = TimerManager::GetInstance()->SetTimer(e, 0.1f, false);
}

void SceneTransition::LoadScene() {
    TimerManager::GetInstance()->ClearTimer(m_TimerHandle);
    SceneManager::GetInstance()->LoadScene(m_Scene);
    AudioSystem::GetInstance()->StopAllChannels();
}
