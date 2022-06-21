#include "Intro.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Scene/Component/UI/ImageComponent.h"
#include "Scene/Component/TransformComponent.h"
#include "Math/Math.h"
#include "Time/Time.h"
#include "Time/TimerManager.h"

Intro::Intro(Actor *owner) : GameComponent(owner) {

}

void Intro::Start() {

    m_FadeUI = m_Owner->GetScene()->GetComponent<ImageComponent>(m_FadeUIID);

    Event event;
//    event.Add(&Intro::FadeIn, this);

//    m_Timer1 = TimerManager::GetInstance()->SetTimer(event, 5.0f, false);

}

void Intro::Update(float deltaTime) {
    if (run) {
        run = false;
        FadeIn(deltaTime);
    }
}

void Intro::FadeIn(float deltaTime) {
//    TimerManager::GetInstance()->ClearTimer(m_Timer1);

    float a = 0.0f;
    float b = 255.0f;
    float x = 5.0f;
    float n = 0.0f;
    float f = 0.0f;

    while(f <= x) {
        n = Math::Lerp(a, b, f / x);
        m_FadeUI->SetColor(glm::vec4(0,0,0,n));
        f += deltaTime;
    }
}

void Intro::FadeOut() {

}
