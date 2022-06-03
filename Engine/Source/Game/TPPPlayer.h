#pragma once

#include "GameComponent.h"
#include "Saveable.h"
#include "Player.h"

class TPPCharacterController;
class Checkpoint;
class CameraController;
class BoxColliderComponent;

class TPPPlayer : public Player
{
public:
	TPPPlayer(Actor* owner);

	void Start() override;
	void Update(float deltaTime) override;

private:
	void MoveForward(float value) override;
	void MoveRight(float value) override;

    void Jump()override;
    void AllowJumping() override;

    void RunOn() override;
    void RunOff() override;

    void Dash() override;
    void AllowDashing() override;

    void Teleport() override;
    void AllowTeleporting() override;

    void HandleHUD() override;

private:
	/* References */
	Ref<TPPCharacterController> m_CharacterController;
	Ref<CameraController> m_CameraController;

	/* Inputs */
    glm::vec3 m_InputDirection;

#pragma region Serialization

    uint64_t m_CameraControllerID;

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};