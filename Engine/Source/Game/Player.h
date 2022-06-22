#pragma once

#include "GameComponent.h"
#include "Saveable.h"

class CharacterController;
class Checkpoint;
class CameraComponent;
class BoxColliderComponent;
class Trail;
class Interactable;
class InteractionPanel;
class Inventory;
class Ghost;
enum class SkillType;

class Player : public GameComponent, public Saveable
{
public:
	Player(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;

	virtual const SaveData Save() override;
	virtual void Load(const SaveData& data) override;

	void SetLastCheckpoint(glm::vec3 position);
	void BackToLastCheckpoint();

	void AddSkill(SkillType skill);
	void SlowDown();
	void BackToNormal();

	virtual float GetMovementSpeed();

	virtual void UpdateGhostAnimatorParams();


	inline Ref<Ghost> GetGhost() const { return m_Ghost; }
	inline Ref<Trail> GetTrail() const { return m_Trail; }
	inline Ref<CharacterController> GetCharacterController() const { return m_CharacterController; }

    inline bool IsRunning() const { return m_IsRunning; };

	inline bool IsJumping() const { return m_IsJumping; }
	inline bool IsDashing() const { return m_IsDashing; }

protected:
    virtual void MoveForward(float value);

    virtual void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);

    virtual void Jump();
    virtual void AllowJumping();

	virtual void Jump_Gamepad();
	virtual void AllowJumping_Gamepad();

    virtual void RunOn();
    virtual void RunOff();

    virtual void Dash();
    virtual void AllowDashing();

    virtual void Teleport();
    virtual void AllowTeleporting();

	virtual void Interact();
	virtual void AllowInteracting();

	void HandleSkillsCooldowns(float deltaTime);

    virtual void HandleHUD();

	void AddMovementInput(glm::vec3 direction, float value);

	virtual void LookForInteractable();
	void DisplayInteractionPanel(Ref<Interactable> interactable);
	void HideInteractionPanel();

protected:
	/* References */
	Ref<CharacterController> m_CharacterController;
	Ref<Inventory> m_Inventory;
	Ref<CameraComponent> m_Camera;
	Ref<Ghost> m_Ghost;
	Ref<Trail> m_Trail;
	Ref<InteractionPanel> m_InteractionPanel;
	
	/* Parameters */
	float m_DashCooldown;
	float m_TeleportCooldown;
	float m_TeleportTime;
	float m_InteractDistance;
	float m_GamepadRotationSensitivity;

	/* Inputs */
	glm::vec3 m_MoveDirection;
	glm::vec3 m_Rotation;

	/* Flags */
	bool m_IsRunning;
	bool m_IsSlowedDown;
	bool m_IsTeleporting;
	bool m_CanJump;
	bool m_CanJump_Gamepad;
	bool m_CanDash;
	bool m_CanTeleport;
	bool m_CanInteract;
	bool m_HasDoubleJumpSkill;
	bool m_HasDashSkill;
	bool m_HasTeleportSkill;

	/* Animator */
	bool m_IsJumping;
	bool m_IsDashing;

	/* Ghost Animator */
	bool m_IsGhostJumping;
	bool m_IsGhostDashing;

	/* Others */
	Ref<Interactable> m_Interactable;
	glm::vec3 m_LastCheckpointPosition;
	glm::vec3 m_TeleportDestinationPosition;
	glm::vec3 m_LastPosition;
	glm::vec3 m_MoveDirectionCopy;
	float m_DashCooldownTimer;
	float m_TeleportCooldownTimer;

#pragma region Serialization

	uint64_t m_CameraID;
	uint64_t m_GhostID;
	uint64_t m_TrailID;
	uint64_t m_InteractionPanelID;

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};