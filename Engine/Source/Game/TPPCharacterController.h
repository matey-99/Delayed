#pragma once

#include "GameComponent.h"
#include "CharacterController.h"

class CameraController;
class BoxColliderComponent;
struct CharacterMovementParams;

class TPPCharacterController : public CharacterController
{
public:
	TPPCharacterController(Actor* owner);

	void Move(glm::vec3 direction, const CharacterMovementParams& params, float deltaTime) override;
	void Rotate(Ref<CameraController> camera, glm::vec3 inputDirection, float deltaTime);

#pragma region Serialization

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};