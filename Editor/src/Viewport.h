#pragma once

#include "Scene/Scene.h"
#include "imgui.h"

class Editor;

class Viewport
{
private:
	Ref<Editor> m_Editor;
	Ref<Scene> m_Scene;

	glm::vec2 m_Size;
	bool m_Hovered;

public:
	Viewport(Ref<Editor> editor, Ref<Scene> scene);

	void Render(Ref<Framebuffer> framebuffer);

	inline bool IsHovered() const { return m_Hovered; }
};