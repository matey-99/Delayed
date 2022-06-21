#include "Intro.h"

#include "Scene/Actor.h"
#include "Scene/Component/UI/ImageComponent.h"
#include "Scene/Component/TransformComponent.h"
#include "Scene/Component/Particle/ParticleSystemComponent.h"
#include "Math/Math.h"
#include "Scene/SceneManager.h"

Intro::Intro(Actor *owner) : GameComponent(owner) {
    m_Time = 0.0f;
    m_Phase = 0;
}

void Intro::Start() {

    m_FadeUI = m_Owner->GetScene()->GetComponent<ImageComponent>(m_FadeUIID);
    m_Spaceship = m_Owner->GetScene()->GetComponent<TransformComponent>(m_SpaceshipID);
    m_Meteor = m_Owner->GetScene()->GetComponent<TransformComponent>(m_MeteorID);
    m_Planet = m_Owner->GetScene()->FindActor(m_PlanetID);
    m_Thruster = m_Owner->GetScene()->GetComponent<ParticleSystemComponent>(m_ThrusterID);

    m_SpaceshipPosition = m_Spaceship->GetLocalPosition();
    m_MeteorPosition = m_Meteor->GetLocalPosition();
}

void Intro::Update(float deltaTime) {

    switch (m_Phase) {
        case 0:
            FadeOut(2.0f);
            break;
        case 1:
            Move(m_Spaceship, 5.0f, m_SpaceshipPosition, m_SpaceshipPosition + glm::vec3(200.0f, 0.0f, 0.0f), true);
            break;
        case 2:
            FadeIn(2.0f);
            m_Spaceship->SetLocalPosition(m_SpaceshipPosition);
            break;
        case 3:
            FadeOut(2.0f);
            break;
        case 4:
            Move(m_Spaceship, 3.0f, m_SpaceshipPosition, m_SpaceshipPosition + glm::vec3(200.0f, 0.0f, 0.0f), true);
            Move(m_Meteor, 3.0f, m_MeteorPosition, m_SpaceshipPosition + glm::vec3(200.0f, 0.0f, 0.0f), false);
            break;
        case 5:
            FadeIn(2.0f);
            break;
        case 6:
            m_Spaceship->SetLocalPosition(m_SpaceshipPosition);
            m_Meteor->GetOwner()->SetEnabled(false);
            m_Planet->SetEnabled(true);
            m_Thruster->SetStartParticleColor(glm::vec4(10.0f, 0.0f, 0.0f, 1.0f));
            m_Thruster->SetEndParticleColor(glm::vec4(10.0f, 10.0f, 0.0f, 1.0f));
            Wait(2.0f);
            break;
        case 7:
            FadeOut(2.0f);
            break;
        case 8:
            Move(m_Spaceship, 10.0f, m_SpaceshipPosition, m_SpaceshipPosition + glm::vec3(500.0f, -500.0f, -700.0f), false);
            FadeIn(10.0f);
            break;
        case 9:
            SceneManager::GetInstance()->LoadScene("Scenes/Transition.scene");
            m_Phase++;
            break;
        default:
            break;
    }

    m_Time += deltaTime;
}

void Intro::FadeIn(float duration) {
    float alpha = m_Time / duration;
    m_FadeUI->SetColor(Math::Lerp(glm::vec4(0, 0, 0, 0), glm::vec4(0, 0, 0, 1), alpha));
    if (m_Time > duration)
        ChangePhase();
}

void Intro::FadeOut(float duration) {
    float alpha = m_Time / duration;
    m_FadeUI->SetColor(Math::Lerp(glm::vec4(0, 0, 0, 1), glm::vec4(0, 0, 0, 0), alpha));
    if (m_Time > duration)
        ChangePhase();
}

void Intro::Move(const Ref<TransformComponent>& transform,float duration, glm::vec3 from, glm::vec3 to, bool next) {
    float alpha = m_Time / duration;
    transform->SetLocalPosition(Math::Lerp(from, to, alpha));
    if (m_Time > duration && next)
        ChangePhase();
}

void Intro::Wait(float duration) {
    if (m_Time > duration)
        ChangePhase();
}

void Intro::ChangePhase() {
    m_Phase++;
    m_Time = 0.0f;
}
