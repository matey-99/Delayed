#pragma once

#include "Core.h"
#include "GameComponent.h"

class Ghost;
class Footsteps;
class CameraComponent;

class PerspectiveController : public GameComponent
{
private:
	enum class PerspectiveType
	{
		FPP, TPP
	};

public:
	PerspectiveController(Actor* owner);

	virtual void Start() override;

	inline static PerspectiveController* GetInstance() { return s_Instance; }

private:
	void ChangePerspective();
	void AllowChangePerspective();

	void ChangePerspective_Gamepad();
	void AllowChangePerspective_Gamepad();

	void SetFPP();
	void SetTPP();

private:
	static PerspectiveController* s_Instance;

	bool m_CanChangePerspective;
	bool m_CanChangePerspective_Gamepad;
	PerspectiveType m_CurrentPerspective;
	
	/* FPP */
	Ref<Actor> m_PlayerFPP;
	Ref<CameraComponent> m_CameraFPP;

	/* TPP */
	Ref<Actor> m_PlayerTPP;
	Ref<CameraComponent> m_CameraTPP;
	Ref<Actor> m_CameraController;

	Ref<Ghost> m_Ghost;

#pragma region Serialization

	uint64_t m_PlayerFPPID;
	uint64_t m_CameraFPPID;
	
	uint64_t m_PlayerTPPID;
	uint64_t m_CameraTPPID;
	uint64_t m_CameraControllerID;

#pragma endregion

	friend class SceneSerializer;
};