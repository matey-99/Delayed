#include "SceneHierarchyPanel.h"

#include "Editor.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/Particle/ParticleSystemComponent.h"
#include "Scene/Component/Light/PointLight.h"
#include "Scene/Component/Light/SpotLight.h"

SceneHierarchyPanel::SceneHierarchyPanel(Ref<Editor> editor, Ref<Scene> scene) : m_Editor(editor), m_Scene(scene)
{
	m_SelectedActor = Ref<Actor>();
	m_SearchActor = "";
}

void SceneHierarchyPanel::Render()
{
	ImGui::Begin("Scene Hierarchy");

	size_t maxSize = 128;
	char* buf = (char*)m_SearchActor.c_str();
	ImGui::InputText("Search", buf, maxSize);
	m_SearchActor = buf;

	if (!m_SearchActor.empty())
	{
		int i = 0;
		for (auto& actor : m_Scene->GetActors())
		{
			std::string name = actor->GetName();

			std::string lowerCaseName = name;
			std::string upperCaseName = name;
			std::transform(name.begin(), name.end(), lowerCaseName.begin(), [] (unsigned char c) { return std::tolower(c); });
			std::transform(name.begin(), name.end(), upperCaseName.begin(), [] (unsigned char c) { return std::toupper(c); });

			if (lowerCaseName.find(m_SearchActor) != std::string::npos || upperCaseName.find(m_SearchActor) != std::string::npos)
			{
				ImGui::PushID(i);
				bool enable = actor->IsEnabled();
				if (ImGui::Checkbox("", &enable))
					actor->SetEnabled(enable);

				ImGui::PopID();

				ImGui::SameLine();

				bool open = false;
				open = ImGui::TreeNodeEx(actor->GetName().c_str(), ImGuiTreeNodeFlags_Leaf);

				if (ImGui::IsItemClicked())
				{
					SelectActor(actor);
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("actor"))
					{
						Ref<Actor>* childActor = static_cast<Ref<Actor>*>(payload->Data);
						childActor->get()->GetTransform()->SetParent(actor->GetTransform().get());
					}
					ImGui::EndDragDropTarget();
				}

				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("actor", &actor, sizeof(Actor));
					ImGui::Text(actor->GetName().c_str());
					ImGui::EndDragDropSource();
				}

				ImGui::TreePop();
			}
		}

		ImGui::End();
		return;
	}


	if (ImGui::TreeNodeEx("Root", ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("actor"))
			{
				Ref<Actor>* childActor = static_cast<Ref<Actor>*>(payload->Data);
				ChangeParent(childActor->get()->GetTransform().get(), m_Scene->GetRoot()->GetTransform().get());
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsItemClicked())
			m_Editor->ShowDetails(m_Scene->GetRoot());

		TreeChildren(m_Scene->GetRoot());

		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("UI Root", ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("actor"))
			{
				Ref<Actor>* childActor = static_cast<Ref<Actor>*>(payload->Data);
				ChangeParent(childActor->get()->GetTransform().get(), m_Scene->GetUIRoot()->GetTransform().get());
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsItemClicked())
			m_Editor->ShowDetails(m_Scene->GetUIRoot());

		TreeChildren(m_Scene->GetUIRoot());
		ImGui::TreePop();
	}

	bool addActor = false;
	bool addUIActor = false;
	if (ImGui::BeginPopupContextWindow())
	{
		ImGui::MenuItem("Add Actor", "", &addActor);
		ImGui::MenuItem("Add UI Actor", "", &addUIActor);

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

	if (addUIActor)
	{
		std::string actorName = "New UI Actor";
		bool unique = false;
		int nr = 1;
		while (!unique)
		{
			unique = true;
			for (auto actor : m_Scene->GetActors())
			{
				if (actor->GetName() == actorName)
				{
					actorName = "New UI Actor (" + std::to_string(nr) + ")";
					nr++;
					unique = false;
				}
			}
		}

		m_Scene->AddUIActor(actorName);
	}

	ImGui::End();
}

void SceneHierarchyPanel::DuplicateSelectedActor()
{
	std::string name = m_SelectedActor->GetName();
	auto parent = m_SelectedActor->GetTransform()->GetParent();

	auto newActor = m_Scene->AddActor(name);
	newActor->GetTransform()->SetParent(parent);

	newActor->GetTransform()->SetLocalPosition(m_SelectedActor->GetTransform()->GetLocalPosition());
	newActor->GetTransform()->SetLocalRotation(m_SelectedActor->GetTransform()->GetLocalRotation());
	newActor->GetTransform()->SetLocalScale(m_SelectedActor->GetTransform()->GetLocalScale());

	if (auto smc = m_SelectedActor->GetComponent<StaticMeshComponent>())
	{
		auto newSMC = newActor->AddComponent<StaticMeshComponent>();
		newSMC->ChangeMesh(smc->GetPath());
		for (int i = 0; i < smc->GetMaterialsPaths().size(); i++)
			newSMC->ChangeMaterial(i, smc->GetMaterialsPaths()[i]);
	}

	if (auto pl = m_SelectedActor->GetComponent<PointLight>())
	{
		auto newPL = newActor->AddComponent<PointLight>(m_Scene->m_LightsVertexUniformBuffer, m_Scene->m_LightsFragmentUniformBuffer);
		newPL->SetColor(pl->GetColor());
	}

	SelectActor(newActor);
}

void SceneHierarchyPanel::DeleteSelectedActor()
{
	auto actor = m_SelectedActor;
	m_Editor->HideDetails();
	UnselectActor();

	m_Scene->RemoveActor(actor.get());
}

void SceneHierarchyPanel::SelectActor(Ref<Actor> actor)
{
	m_SelectedActor = actor;
	m_Editor->ShowDetails(m_SelectedActor);
}

void SceneHierarchyPanel::UnselectActor()
{
	m_Editor->HideDetails();
	m_SelectedActor = Ref<Actor>();
}

void SceneHierarchyPanel::TreeChildren(Ref<Actor> actor)
{
	auto children = actor->GetTransform()->GetChildren();
	for (int i = 0; i < children.size(); i++)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
		if (children[i]->GetChildren().empty())
			flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		else
			flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		bool open = false;
		auto e = m_Scene->FindActor(children[i]->GetOwner()->GetID());

		ImGui::PushID(i);
		bool enable = e->IsEnabled();
		if (ImGui::Checkbox("", &enable))
			e->SetEnabled(enable);

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
				ChangeParent(childActor->get()->GetTransform().get(), children[i]);
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

void SceneHierarchyPanel::ChangeParent(TransformBaseComponent* child, TransformBaseComponent* parent)
{

	child->SetLocalPosition(child->GetLocalPosition() + child->GetParent()->GetLocalPosition());
	child->SetLocalScale(child->GetLocalScale() * child->GetParent()->GetLocalScale());

	child->SetParent(parent);

	child->SetLocalPosition((child->GetLocalPosition() - parent->GetLocalPosition()) / parent->GetLocalScale());
	child->SetLocalScale(child->GetLocalScale() / parent->GetLocalScale());
}
