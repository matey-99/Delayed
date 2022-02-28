#include "SceneHierarchyPanel.h"

#include "Editor.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/InstanceRenderedMeshComponent.h"
#include "Scene/Component/ParticleSystemComponent.h"
#include "Scene/Component/Light/PointLight.h"
#include "Scene/Component/Light/SpotLight.h"

SceneHierarchyPanel::SceneHierarchyPanel(Ref<Editor> editor, Ref<Scene> scene) : m_Editor(editor), m_Scene(scene)
{
	m_SelectedActor = Ref<Actor>();
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

	bool addActor = false;
	if (ImGui::BeginPopupContextWindow())
	{
		ImGui::MenuItem("Add Actor", "", &addActor);

		ImGui::EndPopup();
	}

	if (addActor)
	{
		std::string actorName = "New Actor";
		bool unique = false;
		int nr = 1;
		while (!unique)
		{
			unique = true;
			for (auto actor : m_Scene->GetActors())
			{
				if (actor->GetName() == actorName)
				{
					actorName = "New Actor (" + std::to_string(nr) + ")";
					nr++;
					unique = false;
				}
			}
		}

		m_Scene->AddActor(actorName);
	}

	ImGui::End();
}

void SceneHierarchyPanel::DuplicateSelectedActor()
{
	std::string name = m_SelectedActor->GetName();
	Actor* parent = m_SelectedActor->GetParent();

	auto newActor = m_Scene->AddActor(name);
	newActor->SetParent(parent);

	newActor->SetLocalPosition(m_SelectedActor->GetTransform().LocalPosition);
	newActor->SetLocalRotation(m_SelectedActor->GetTransform().LocalRotation);
	newActor->SetLocalScale(m_SelectedActor->GetTransform().LocalScale);

	if (auto smc = m_SelectedActor->GetComponent<StaticMeshComponent>())
	{
		auto newSMC = newActor->AddComponent<StaticMeshComponent>();
		newSMC->ChangeMesh(smc->GetPath());
		for (int i = 0; i < smc->GetMaterialsPaths().size(); i++)
			newSMC->ChangeMaterial(i, smc->GetMaterialsPaths()[i]);
	}

	if (auto irmc = m_SelectedActor->GetComponent<InstanceRenderedMeshComponent>())
	{
		auto newIRMC = newActor->AddComponent<InstanceRenderedMeshComponent>();
		newIRMC->ChangeMesh(irmc->GetPath());
		for (int i = 0; i < irmc->GetMaterialsPaths().size(); i++)
			newIRMC->ChangeMaterial(i, irmc->GetMaterialsPaths()[i]);

		newIRMC->SetInstancesCount(irmc->GetInstancesCount());
		newIRMC->SetRadius(irmc->GetRadius());
		newIRMC->SetMinMeshScale(irmc->GetMinMeshScale());
		newIRMC->SetMaxMeshScale(irmc->GetMaxMeshScale());
	}

	SelectActor(newActor);
}

void SceneHierarchyPanel::SelectActor(Ref<Actor> actor)
{
	m_SelectedActor = actor;
	m_Editor->ShowDetails(m_SelectedActor);
}

void SceneHierarchyPanel::UnselectActor()
{
	m_SelectedActor = Ref<Actor>();
}

void SceneHierarchyPanel::TreeChildren(Ref<Actor> actor)
{
	auto children = actor->m_Children;
	for (int i = 0; i < children.size(); i++)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
		if (children[i]->m_Children.empty())
			flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		else
			flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		bool open = false;
		auto e = m_Scene->FindActor(children[i]->m_ID);

		ImGui::PushID(i);
		bool enable = e->IsEnable();
		if (ImGui::Checkbox("", &enable))
			e->SetEnable(enable);

		ImGui::PopID();

		ImGui::SameLine();
		open = ImGui::TreeNodeEx(e->GetName().c_str(), flags);

		if (ImGui::IsItemClicked())
		{
			SelectActor(e);
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("actor"))
			{
				Ref<Actor>* childActor = static_cast<Ref<Actor>*>(payload->Data);
				childActor->get()->SetParent(children[i]);
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("actor", &e, sizeof(Actor));
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
