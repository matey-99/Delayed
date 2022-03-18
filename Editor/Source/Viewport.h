#pragma once

#include "Scene/Scene.h"
#include "imgui.h"
#include "Renderer/Mesh.h"

class Editor;

class Viewport
{
private:
	Ref<Editor> m_Editor;
	Ref<Scene> m_Scene;

	glm::vec2 m_Size;
	bool m_Hovered;

	Ref<Shader> m_DirectionArrowShader;
	std::vector<Mesh> m_DirectionArrow;

public:
	Viewport(Ref<Editor> editor, Ref<Scene> scene);

	void Render();
	void RenderGizmos();

	inline bool IsHovered() const { return m_Hovered; }
};