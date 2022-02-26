#pragma once

#include "Scene/Scene.h"
#include "imgui.h"

class Editor;

class CameraComponentViewport
{
private:
	Ref<Editor> m_Editor;
	Ref<Scene> m_Scene;

	glm::vec2 m_Size;

	Ref<Framebuffer> m_ViewportFramebuffer;

public:
	CameraComponentViewport(Ref<Editor> editor, Ref<Scene> scene);

	void Render(Ref<Framebuffer> framebuffer, Ref<CameraComponent> camera);
};