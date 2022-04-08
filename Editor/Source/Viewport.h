#pragma once

#include "Scene/Scene.h"
#include "imgui.h"
#include "Renderer/StaticMesh.h"
#include "Renderer/RenderTarget.h"

class Editor;

class Viewport
{
private:
	Ref<Editor> m_Editor;
	Ref<Scene> m_Scene;

	glm::vec2 m_Size;
	bool m_Hovered;

	Ref<RenderTarget> m_GizmosRenderTarget;
	Ref<RenderTarget> m_ViewportRenderTarget;

	Ref<Shader> m_DirectionArrowShader;
	Ref<Shader> m_ColliderShader;
	Ref<Shader> m_ViewportShader;
	std::vector<Ref<StaticMesh>> m_DirectionArrow;

	bool m_SelectingActor;

public:
	Viewport(Ref<Editor> editor, Ref<Scene> scene);

	void Render();
	void RenderGizmos();

	inline bool IsHovered() const { return m_Hovered; }
	inline bool IsSelectingActor() const { return m_SelectingActor; }
};