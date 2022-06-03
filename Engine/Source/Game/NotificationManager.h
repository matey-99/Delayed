#pragma once

#include "Core.h"
#include "GameComponent.h"

class TextComponent;

class NotificationManager : public GameComponent
{
public:
	NotificationManager(Actor* owner);

	virtual void Start() override;

	void Notify(std::string message, glm::vec4 color = glm::vec4(1.0f), float timeToHide = 3.0f);

	inline static NotificationManager* GetInstance() { return s_Instance; }

private:
	void HideNotification();

private:
	static NotificationManager* s_Instance;

	Ref<TextComponent> m_NotificationText;
	TimerHandle m_NotificationTimerHandle;

#pragma region Serialization

	uint64_t m_NotificationTextID;

#pragma endregion

	friend class SceneSerializer;
};