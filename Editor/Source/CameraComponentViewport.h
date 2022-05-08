#pragma once

#include "Scene/Scene.h"
#include "imgui.h"

class Editor;
class RenderTarget;

class CameraComponentViewport
{
private:
	Ref<Editor> m_Editor;
	Ref<Scene> m_Scene;

	Ref<RenderTarget> m_RenderTarget;

	glm::vec2 m_Size;

public:
	CameraComponentViewport(Ref<Editor> editor, Ref<Scene> scene);

	void Render(Ref<CameraComponent> camera);
};