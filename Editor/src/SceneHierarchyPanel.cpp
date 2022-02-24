#include "SceneHierarchyPanel.h"

#include "Editor.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/InstanceRenderedMeshComponent.h"
#include "Scene/Component/ParticleSystemComponent.h"
#include "Scene/Component/Light/PointLight.h"
#include "Scene/Component/Light/SpotLight.h"

SceneHierarchyPanel::SceneHierarchyPanel(Ref<Editor> editor, Ref<Scene> scene) : m_Editor(editor), m_Scene(scene)
{
	m_SelectedEntity = Ref<Entity>();
}

void SceneHierarchyPanel::Render()
{
	ImGui::Begin("Scene Hierarchy");

	if (ImGui::TreeNodeEx("Root", ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::IsItemClicked())
			m_Editor->ShowDetails(m_Scene->GetRoot());

		TreeChildren(m_Scene->GetRoot());
		ImGui::TreePop();
	}

	bool addEntity = false;
	if (ImGui::BeginPopupContextWindow())
	{
		ImGui::MenuItem("Add Entity", "", &addEntity);

		ImGui::EndPopup();
	}

	if (addEntity)
	{
		std::string entityName = "New Entity";
		bool unique = false;
		int nr = 1;
		while (!unique)
		{
			unique = true;
			for (auto entity : m_Scene->GetEntities())
			{
				if (entity->GetName() == entityName)
				{
					entityName = "New Entity (" + std::to_string(nr) + ")";
					nr++;
					unique = false;
				}
			}
		}

		m_Scene->AddEntity(entityName);
	}

	ImGui::End();
}

void SceneHierarchyPanel::DuplicateSelectedEntity()
{
	std::string name = m_SelectedEntity->GetName();
	Entity* parent = m_SelectedEntity->GetParent();

	auto newEntity = m_Scene->AddEntity(name);
	newEntity->SetParent(parent);

	newEntity->SetLocalPosition(m_SelectedEntity->GetTransform().LocalPosition);
	newEntity->SetLocalRotation(m_SelectedEntity->GetTransform().LocalRotation);
	newEntity->SetLocalScale(m_SelectedEntity->GetTransform().LocalScale);

	if (auto smc = m_SelectedEntity->GetComponent<StaticMeshComponent>())
	{
		auto newSMC = newEntity->AddComponent<StaticMeshComponent>();
		newSMC->ChangeMesh(smc->GetPath());
		for (int i = 0; i < smc->GetMaterialsPaths().size(); i++)
			newSMC->ChangeMaterial(i, smc->GetMaterialsPaths()[i]);
	}

	if (auto irmc = m_SelectedEntity->GetComponent<InstanceRenderedMeshComponent>())
	{
		auto newIRMC = newEntity->AddComponent<InstanceRenderedMeshComponent>();
		newIRMC->ChangeMesh(irmc->GetPath());
		for (int i = 0; i < irmc->GetMaterialsPaths().size(); i++)
			newIRMC->ChangeMaterial(i, irmc->GetMaterialsPaths()[i]);

		newIRMC->SetInstancesCount(irmc->GetInstancesCount());
		newIRMC->SetRadius(irmc->GetRadius());
		newIRMC->SetMinMeshScale(irmc->GetMinMeshScale());
		newIRMC->SetMaxMeshScale(irmc->GetMaxMeshScale());
	}

	SelectEntity(newEntity);
}

void SceneHierarchyPanel::SelectEntity(Ref<Entity> entity)
{
	m_SelectedEntity = entity;
	m_Editor->ShowDetails(m_SelectedEntity);
}

void SceneHierarchyPanel::UnselectEntity()
{
	m_SelectedEntity = Ref<Entity>();
}

void SceneHierarchyPanel::TreeChildren(Ref<Entity> entity)
{
	auto children = entity->m_Children;
	for (int i = 0; i < children.size(); i++)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
		if (children[i]->m_Children.empty())
			flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		else
			flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		bool open = false;
		auto e = m_Scene->FindEntity(children[i]->m_ID);

		ImGui::PushID(i);
		bool enable = e->IsEnable();
		if (ImGui::Checkbox("", &enable))
			e->SetEnable(enable);

		ImGui::PopID();

		ImGui::SameLine();
		open = ImGui::TreeNodeEx(e->GetName().c_str(), flags);

		if (ImGui::IsItemClicked())
		{
			SelectEntity(e);
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("entity"))
			{
				Ref<Entity>* childEntity = static_cast<Ref<Entity>*>(payload->Data);
				childEntity->get()->SetParent(children[i]);
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("entity", &e, sizeof(Entity));
			ImGui::Text(e->GetName().c_str());
			ImGui::EndDragDropSource();
		}

		if (!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen) && open)
		{
			TreeChildren(e);
			ImGui::TreePop();
		}
	}
}
