#include "RigidBodyComponent.h"
#include "Scene/Actor.h"
#include "Time/Time.h"

RigidBodyComponent::RigidBodyComponent(Actor *owner) : Component(owner) {

    m_Gravity = -1.0f;
    m_Mass = 1.0f;
    m_Drag = 0.94f;
    m_Velocity = glm::vec3(0.0f);
    m_VelocityMax = glm::vec3(10.0f);
    m_Acceleration = glm::vec3(0.0f);
}

void RigidBodyComponent::Start() {

}

void RigidBodyComponent::Update(float deltaTime) {


}

void RigidBodyComponent::FixedUpdate()
{
    m_Acceleration.y += m_Gravity;

    m_Velocity += m_Acceleration;

    m_Velocity *= m_Drag;

    m_Owner->GetTransform()->SetLocalPosition(m_Owner->GetTransform()->GetLocalPosition() + glm::clamp(m_Velocity * Time::GetInstance()->GetFixedDeltaTime(), -m_VelocityMax, m_VelocityMax));

    // Reset acceleration
    m_Acceleration = glm::vec3(0.0f);
}

void RigidBodyComponent::Destroy() {

}

void RigidBodyComponent::AddForce(glm::vec3 force) {
    m_Acceleration += force;
}