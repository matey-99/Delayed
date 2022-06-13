#pragma once

#include "GameComponent.h"
#include "Saveable.h"

class CharacterController;
class Checkpoint;
class CameraComponent;
class BoxColliderComponent;
class Trail;
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

	inline Ref<Actor> GetGhost() const { return m_Ghost; }
	inline Ref<Trail> GetTrail() const { return m_Trail; }
	inline Ref<CharacterController> GetCharacterController() const { return m_CharacterController; }

    inline bool IsRunning() const { return m_IsRunning; };

private:
	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);

	void Jump();
	void AllowJumping();

	void RunOn();
	void RunOff();

	void Dash();
	void AllowDashing();

	void Teleport();
	void AllowTeleporting();

	void HandleSkillsCooldowns(float deltaTime);
	void HandleHUD();

	void AddMovementInput(glm::vec3 direction, float value);

private:
	/* References */
	Ref<CharacterController> m_CharacterController;
	Ref<CameraComponent> m_Camera;
	Ref<Actor> m_Ghost;
	Ref<Trail> m_Trail;
	Ref<Actor> m_StaminaBar;
	
	/* Parameters */
	float m_DashCooldown;
	float m_TeleportCooldown;
	float m_TeleportTime;

	/* Inputs */
	glm::vec3 m_MoveDirection;
	glm::vec3 m_Rotation;

	/* Flags */
	bool m_IsRunning;
	bool m_IsJumping;
	bool m_IsSlowedDown;
	bool m_IsTeleporting;
	bool m_CanJump;
	bool m_CanDash;
	bool m_CanTeleport;
	bool m_HasDoubleJumpSkill;
	bool m_HasDashSkill;
	bool m_HasTeleportSkill;

	/* Others */
	glm::vec3 m_LastCheckpointPosition;
	glm::vec3 m_StaminaBarDefaultScale;
	glm::vec3 m_TeleportDestinationPosition;
	glm::vec3 m_LastPosition;
	glm::vec3 m_MoveDirectionCopy;
	float m_DashCooldownTimer;
	float m_TeleportCooldownTimer;

#pragma region Serialization

	uint64_t m_CameraID;
	uint64_t m_GhostID;
	uint64_t m_TrailID;
	uint64_t m_StaminaBarID;

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};