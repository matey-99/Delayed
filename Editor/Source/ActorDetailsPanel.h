#pragma once

#include "Scene/Actor.h"

class Editor;

class ActorDetailsPanel
{
private:
	Ref<Editor> m_Editor;
	Ref<Actor> m_Actor;

public:
	ActorDetailsPanel(Ref<Editor> editor);
	void Render();

	inline void SetActor(Ref<Actor> actor) { m_Actor = actor; }

private:
	void DisplayResources(Ref<Component> component, std::vector<std::string> extensions, int index = -1, int lod = -1);
	void CorrectPath(std::string& path);
};