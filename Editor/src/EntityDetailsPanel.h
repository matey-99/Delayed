#pragma once

#include "Scene/Entity.h"

class Editor;


class EntityDetailsPanel
{
private:
	Ref<Editor> m_Editor;
	Ref<Entity> m_Entity;

public:
	EntityDetailsPanel(Ref<Editor> editor);
	void Render();

	inline void SetEntity(Ref<Entity> entity) { m_Entity = entity; }

private:
	void DisplayResources(std::vector<std::string> extensions, int index = -1);
	void CorrectPath(std::string& path);
};