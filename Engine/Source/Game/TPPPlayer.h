#pragma once

#include "GameComponent.h"
#include "Saveable.h"
#include "Player.h"

class TPPCharacterController;
class Checkpoint;
class CameraController;
class BoxColliderComponent;
class Animator;

class TPPPlayer : public Player
{
public:
	TPPPlayer(Actor* owner);

	void Start() override;
	void Update(float deltaTime) override;

    virtual float GetMovementSpeed() override;

    inline Ref<Animator> GetCharacterAnimator() const { return m_CharacterAnimator; }

    inline Ref<TPPCharacterController> GetCharacterController() const { return m_CharacterController; }

private:
	void MoveForward(float value) override;
	void MoveRight(float value) override;

    void Jump()override;
    void AllowJumping() override;

    void Jump_Gamepad() override;
    void AllowJumping_Gamepad() override;

    void RunOn() override;
    void RunOff() override;

    void Dash() override;
    void AllowDashing() override;

    void Teleport() override;
    void AllowTeleporting() override;

    void Interact() override;
    void AllowInteracting() override;

    void LookForInteractable() override;

    void HandleHUD() override;

    void HandleAnimator();

private:
	/* References */
	Ref<TPPCharacterController> m_CharacterController;
	Ref<CameraController> m_CameraController;
    Ref<Animator> m_CharacterAnimator;

	/* Inputs */
    glm::vec3 m_InputDirection;

    /* Others */
    bool m_LastFrameIsGrounded;

#pragma region Serialization

    uint64_t m_CameraControllerID;

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};