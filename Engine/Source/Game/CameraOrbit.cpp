#include "CameraOrbit.h"

#include "Scene/Scene.h"
#include "Scene/Actor.h"
#include "Scene/Component/CameraComponent.h"
#include "Scene/Component/TransformComponent.h"
#include "Camera/CameraManager.h"
#include "Math/Math.h"
#include "Time/Time.h"

CameraOrbit::CameraOrbit(Actor *owner) : GameComponent(owner) {
    m_Speed = 1.0f;
}

void CameraOrbit::Start() {
}

void CameraOrbit::Update(float deltaTime) {
    // Turn Rotation
    glm::vec3 newRotation = m_Owner->GetTransform()->GetLocalRotation();
    newRotation.y += m_Speed * deltaTime;
    m_Owner->GetTransform()->SetLocalRotation(newRotation);
}

