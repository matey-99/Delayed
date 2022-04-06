#include "SceneSerializer.h"

#include "yaml/yaml.h"
#include "Content/ContentHelper.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/SkeletalMeshComponent.h"
#include "Scene/Component/InstanceRenderedMeshComponent.h"
#include "Scene/Component/Light/DirectionalLight.h"
#include "Scene/Component/Light/PointLight.h"
#include "Scene/Component/Light/SpotLight.h"
#include "Scene/Component/Light/SkyLight.h"
#include "Scene/Component/ParticleSystemComponent.h"
#include "Scene/Component/Collider/BoxColliderComponent.h"
#include <Scene/Component/Collider/SphereColliderComponent.h>
#include <Scene/Component/RigidBodyComponent.h>
#include "Scene/Component/UI/ImageComponent.h"
#include "Scene/Component/UI/ButtonComponent.h"
#include "Scene/Component/UI/RectTransformComponent.h"

#include "Game/MainMenu.h"
#include "Game/Player.h"
#include "Game/Button.h"

void SceneSerializer::Serialize(Ref<Scene> scene, std::string destinationPath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << scene->m_Name;
	out << YAML::Key << "CurrentCamera" << YAML::Value << scene->m_CurrentCamera->GetOwner()->GetID();

	out << YAML::Key << "RendererSettings" << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "FXAA" << YAML::Value << Renderer::GetInstance()->GetSettings().FXAAEnabled;
	out << YAML::Key << "DepthOfField" << YAML::Value << Renderer::GetInstance()->GetSettings().DepthOfFieldEnabled;
	out << YAML::EndMap;

	out << YAML::Key << "Actors" << YAML::Value << YAML::BeginSeq;
	for (auto actor : scene->GetActors())
	{
		SerializeActor(out, actor);
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream file(destinationPath);
	file << out.c_str();
	file.close();
}

Ref<Scene> SceneSerializer::Deserialize(std::string path)
{
	Ref<Scene> scene = CreateRef<Scene>();

	std::ifstream file(path);
	std::stringstream ss;
	ss << file.rdbuf();

	YAML::Node data = YAML::Load(ss.str());
	if (!data["Scene"])
	{
		std::cout << "Cannot load scene!" << std::endl;
		return Ref<Scene>();
	}
	
	std::string sceneName = data["Scene"].as<std::string>();
	scene->m_Name = sceneName;

	bool fxaa = data["RendererSettings"]["FXAA"].as<bool>();
	bool depthOfField = data["RendererSettings"]["DepthOfField"].as<bool>();

	Renderer::RendererSettings settings;
	settings.FXAAEnabled = fxaa;
	settings.DepthOfFieldEnabled = depthOfField;

	Renderer::GetInstance()->SetSettings(settings);

	YAML::Node actors = data["Actors"];
	if (actors)
	{
		auto parentsIDs = std::vector<int64_t>();

		for (auto actor : actors)
		{
			if (actor["ID"].as<uint64_t>() == 0 || actor["ID"].as<uint64_t>() == 1)
				continue;

			Ref<Actor> a = Ref<Actor>();

			YAML::Node components = actor["Components"];
			if (components)
			{
				for (auto component : components)
				{
					if (auto transform = component["Transform"])
					{
						a = scene->AddActor(actor["ID"].as<uint64_t>(), actor["Name"].as<std::string>());

						if (auto dynamic = actor["Dynamic"])
							a->SetDynamic(dynamic.as<bool>());
						else
							a->SetDynamic(false);

						auto t = a->GetComponent<TransformComponent>();
						t->SetLocalPosition(transform["LocalPosition"].as<glm::vec3>());
						t->SetLocalRotation(transform["LocalRotation"].as<glm::vec3>());
						t->SetLocalScale(transform["LocalScale"].as<glm::vec3>());

						if (auto parent = transform["Parent"])
						{
							parentsIDs.push_back(parent.as<uint64_t>());
						}
					}
					if (auto rectTransform = component["RectTransform"])
					{
						a = scene->AddUIActor(actor["ID"].as<uint64_t>(), actor["Name"].as<std::string>());
						
						if (auto dynamic = actor["Dynamic"])
							a->SetDynamic(dynamic.as<bool>());
						else
							a->SetDynamic(false);

						auto t = a->GetComponent<RectTransformComponent>();
						t->SetAnchor((AnchorType)rectTransform["Anchor"].as<uint16_t>());
						t->SetLocalPosition(rectTransform["LocalPosition"].as<glm::vec3>());
						t->SetLocalRotation(rectTransform["LocalRotation"].as<glm::vec3>());
						t->SetLocalScale(rectTransform["LocalScale"].as<glm::vec3>());

						if (auto parent = rectTransform["Parent"])
						{
							parentsIDs.push_back(parent.as<uint64_t>());
						}
					}

					if (auto mesh = component["StaticMesh"])
					{
						std::string path = mesh["Mesh"].as<std::string>();
						std::vector<std::string> materialsPaths;
						YAML::Node materials = mesh["Materials"];
						for (auto material : materials)
						{
							materialsPaths.push_back(material["Path"].as<std::string>());
						}
						a->AddComponent<StaticMeshComponent>(path.c_str(), materialsPaths);
					}

					if (auto mesh = component["SkeletalMesh"])
					{
						std::string path = mesh["SkeletalMesh"].as<std::string>();
						std::vector<std::string> materialsPaths;
						YAML::Node materials = mesh["Materials"];
						for (auto material : materials)
						{
							materialsPaths.push_back(material["Path"].as<std::string>());
						}
						a->AddComponent<SkeletalMeshComponent>(path.c_str(), materialsPaths);
					}

					if (auto mesh = component["InstanceRenderedMesh"])
					{
						std::string path = mesh["Mesh"].as<std::string>();
						std::vector<std::string> materialsPaths;
						YAML::Node materials = mesh["Materials"];
						for (auto material : materials)
						{
							materialsPaths.push_back(material["Path"].as<std::string>());
						}

						float radius = mesh["Radius"].as<float>();
						int count = mesh["InstancesCount"].as<int>();
						float minScale = mesh["MinMeshScale"].as<float>();
						float maxScale = mesh["MaxMeshScale"].as<float>();

						auto m = a->AddComponent<InstanceRenderedMeshComponent>(path.c_str(), materialsPaths);
						m->m_Radius = radius;
						m->m_InstancesCount = count;
						m->m_MinMeshScale = minScale;
						m->m_MaxMeshScale = maxScale;
						m->Generate();
					}

					if (auto dirLight = component["DirectionalLight"])
					{
						glm::vec3 color = dirLight["Color"].as<glm::vec3>();

						auto l = a->AddComponent<DirectionalLight>(scene->m_LightsVertexUniformBuffer, scene->m_LightsFragmentUniformBuffer);
						l->SetColor(color);
					}

					if (auto pointLight = component["PointLight"])
					{
						glm::vec3 color = pointLight["Color"].as<glm::vec3>();

						auto l = a->AddComponent<PointLight>(scene->m_LightsVertexUniformBuffer, scene->m_LightsFragmentUniformBuffer);
						l->SetColor(color);
					}

					if (auto spotLight = component["SpotLight"])
					{
						float innerCutOff = spotLight["InnerCutOff"].as<float>();
						float outerCutOff = spotLight["OuterCutOff"].as<float>();
						glm::vec3 color = spotLight["Color"].as<glm::vec3>();

						auto l = a->AddComponent<SpotLight>(scene->m_LightsVertexUniformBuffer, scene->m_LightsFragmentUniformBuffer);
						l->SetInnerCutOff(innerCutOff);
						l->SetOuterCutOff(outerCutOff);
						l->SetColor(color);
					}

					if (auto skyLight = component["SkyLight"])
					{
						std::vector<std::string> paths;
						auto p = skyLight["Paths"];
						uint16_t index = 0;
						for (auto path : p)
							paths.push_back(path.second.as<std::string>());

						glm::vec3 color = skyLight["Color"].as<glm::vec3>();
						float intensity = skyLight["Intensity"].as<float>();

						auto l = a->AddComponent<SkyLight>(paths);
						l->m_Color = color;
						l->m_Intensity = intensity;
					}

					if (auto particle = component["ParticleSystem"])
					{
						int count = particle["ParticlesCount"].as<int>();
						float radius = particle["SphereRadius"].as<float>();
						glm::vec3 minVelocity = particle["MinVelocity"].as<glm::vec3>();
						glm::vec3 maxVelocity = particle["MaxVelocity"].as<glm::vec3>();

						auto p = a->AddComponent<ParticleSystemComponent>();
						p->m_ParticlesCount = count;
						p->m_Radius = radius;
						p->m_MinVelocity = minVelocity;
						p->m_MaxVelocity = maxVelocity;

						p->Reset();
					}

					if (auto camera = component["Camera"])
					{
						glm::vec2 aspectRatio = camera["AspectRatio"].as<glm::vec2>();
						float fieldOfView = camera["FieldOfView"].as<float>();
						float nearClipPlane = camera["NearClipPlane"].as<float>();
						float farClipPlane = camera["FarClipPlane"].as<float>();

						auto c = a->AddComponent<CameraComponent>();
						c->m_AspectRatio = aspectRatio;
						c->m_FieldOfView = fieldOfView;
						c->m_NearClipPlane = nearClipPlane;
						c->m_FarClipPlane = farClipPlane;
					}

					if (auto boxCollider = component["BoxCollider"])
					{
						bool isTrigger = boxCollider["IsTrigger"].as<bool>();
						glm::vec3 center = boxCollider["Center"].as<glm::vec3>();
						glm::vec3 size = boxCollider["Size"].as<glm::vec3>();

						auto b = a->AddComponent<BoxColliderComponent>();
						b->m_IsTrigger = isTrigger;
						b->m_Center = center;
						b->m_Size = size;
					}

					if (auto sphereCollider = component["SphereCollider"]) {
						bool isTrigger = sphereCollider["IsTrigger"].as<bool>();
                        glm::vec3 center = sphereCollider["Center"].as<glm::vec3>();
                        float size = sphereCollider["Size"].as<float>();

                        auto b = a->AddComponent<SphereColliderComponent>();
						b->m_IsTrigger = isTrigger;
                        b->m_Center = center;
                        b->m_Size = size;
                    }

					if (auto rigidBody = component["RigidBody"])
                    {
                        float gravity = rigidBody["Gravity"].as<float>();
                        float mass = rigidBody["Mass"].as<float>();
                        float drag = rigidBody["Drag"].as<float>();

                        auto r = a->AddComponent<RigidBodyComponent>();
                        r->m_Gravity = gravity;
                        r->m_Mass = mass;
                        r->m_Drag = drag;
                    }

					if (auto image = component["Image"])
					{
						std::string path = image["Path"].as<std::string>();
						glm::vec4 color = image["Color"].as<glm::vec4>();

						auto i = a->AddComponent<ImageComponent>();
						i->m_Image = Texture::Create(path);
						i->m_Color = color;
					}

					if (auto button = component["UIButton"])
					{
						bool enabled = button["Enabled"].as<bool>();
						std::string path = button["ImagePath"].as<std::string>();
						glm::vec4 normalColor = button["NormalColor"].as<glm::vec4>();
						glm::vec4 hoveredColor = button["HoveredColor"].as<glm::vec4>();
						glm::vec4 pressedColor = button["PressedColor"].as<glm::vec4>();
						glm::vec4 disabledColor = button["DisabledColor"].as<glm::vec4>();

						auto b = a->AddComponent<ButtonComponent>();
						b->m_Enabled = enabled;
						b->m_Image = Texture::Create(path);
						b->m_NormalColor = normalColor;
						b->m_HoveredColor = hoveredColor;
						b->m_PressedColor = pressedColor;
						b->m_DisabledColor = disabledColor;
					}

					/* --- GAME COMPONENTS --- */

					if (auto menu = component["MainMenu"])
					{
						uint64_t playButtonActorID = menu["PlayButton"].as<uint64_t>();
						uint64_t optionsButtonActorID = menu["OptionsButton"].as<uint64_t>();
						uint64_t exitButtonActorID = menu["ExitButton"].as<uint64_t>();

						auto m = a->AddComponent<MainMenu>();
						m->m_PlayButtonID = playButtonActorID;
						m->m_OptionsButtonID = optionsButtonActorID;
						m->m_ExitButtonID = exitButtonActorID;
					}

					if (auto player = component["Player"])
					{
						uint64_t cameraActorID = player["Camera"].as<uint64_t>();

						auto p = a->AddComponent<Player>();
						p->m_CameraID = cameraActorID;
					}

					if (auto button = component["Button"])
					{
						a->AddComponent<Button>();
					}
				}
			}
		}

		for (int i = 2; i < scene->GetActors().size(); i++)
		{
			scene->GetActors().at(i)->GetTransform()->SetParent(scene->FindActor(parentsIDs[i - 2])->GetTransform().get());
		}

		uint64_t currentCameraID = data["CurrentCamera"].as<uint64_t>();
		scene->m_CurrentCamera = scene->FindActor(currentCameraID)->GetComponent<CameraComponent>();
	}

	file.close();
	return scene;
}

void SceneSerializer::SerializeActor(YAML::Emitter& out, Ref<Actor> actor)
{
	// Root and UI Root ID's
	if (actor->GetID() == 0 || actor->GetID() == 1)
		return;

	out << YAML::BeginMap;
	out << YAML::Key << "Actor" << YAML::Value << actor->GetID();
	out << YAML::Key << "ID" << YAML::Value << actor->GetID();
	out << YAML::Key << "Name" << YAML::Value << actor->GetName();
	out << YAML::Key << "Dynamic" << YAML::Value << actor->IsDynamic();

	out << YAML::Key << "Components" << YAML::Value << YAML::BeginSeq;

	/* --- ENGINE COMPONENTS --- */

	if (auto transform = actor->GetComponent<TransformComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Transform";

		out << YAML::BeginMap;
		out << YAML::Key << "LocalPosition" << YAML::Value << transform->m_LocalPosition;
		out << YAML::Key << "LocalRotation" << YAML::Value << transform->m_LocalRotation;
		out << YAML::Key << "LocalScale" << YAML::Value << transform->m_LocalScale;

		if (transform->GetParent())
			out << YAML::Key << "Parent" << YAML::Value << transform->GetParent()->GetOwner()->GetID();

		out << YAML::EndMap;
		out << YAML::EndMap;
	}
	if (auto rectTransform = actor->GetComponent<RectTransformComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "RectTransform";

		out << YAML::BeginMap;
		out << YAML::Key << "Anchor" << YAML::Value << (uint16_t)rectTransform->m_AnchorType;
		out << YAML::Key << "LocalPosition" << YAML::Value << rectTransform->m_LocalPosition;
		out << YAML::Key << "LocalRotation" << YAML::Value << rectTransform->m_LocalRotation;
		out << YAML::Key << "LocalScale" << YAML::Value << rectTransform->m_LocalScale;

		if (rectTransform->GetParent())
			out << YAML::Key << "Parent" << YAML::Value << rectTransform->GetParent()->GetOwner()->GetID();

		out << YAML::EndMap;
		out << YAML::EndMap;
	}
	if (auto mesh = actor->GetComponent<StaticMeshComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "StaticMesh";

		out << YAML::BeginMap;
		out << YAML::Key << "Mesh" << YAML::Value << mesh->GetPath();
		out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq;
		for (int i = 0; i < mesh->GetMaterialsPaths().size(); i++)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Material" << YAML::Value << i;
			out << YAML::Key << "Path" << YAML::Value << mesh->GetMaterialsPaths().at(i);
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}
	if (auto mesh = actor->GetComponent<SkeletalMeshComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "SkeletalMesh";

		out << YAML::BeginMap;
		out << YAML::Key << "SkeletalMesh" << YAML::Value << mesh->GetPath();
		out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq;
		//out << YAML::Key << "Bones" << YAML::Value << YAML::BeginSeq;
		for (int i = 0; i < mesh->GetMaterialsPaths().size(); i++)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Material" << YAML::Value << i;
			out << YAML::Key << "Path" << YAML::Value << mesh->GetMaterialsPaths().at(i);
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}
	if (auto mesh = actor->GetComponent<InstanceRenderedMeshComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "InstanceRenderedMesh";
		out << YAML::BeginMap;
		out << YAML::Key << "Mesh" << YAML::Value << mesh->GetPath();
		out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq;
		for (int i = 0; i < mesh->GetMaterialsPaths().size(); i++)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Material" << YAML::Value << i;
			out << YAML::Key << "Path" << YAML::Value << mesh->GetMaterialsPaths().at(i);
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;

		out << YAML::Key << "Radius" << YAML::Value << mesh->m_Radius;
		out << YAML::Key << "InstancesCount" << YAML::Value << mesh->m_InstancesCount;
		out << YAML::Key << "MinMeshScale" << YAML::Value << mesh->m_MinMeshScale;
		out << YAML::Key << "MaxMeshScale" << YAML::Value << mesh->m_MaxMeshScale;


		out << YAML::EndMap;
		out << YAML::EndMap;
	}
	if (auto dirLight = actor->GetComponent<DirectionalLight>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "DirectionalLight";
		out << YAML::BeginMap;
		out << YAML::Key << "Color" << YAML::Value << dirLight->GetColor();
		out << YAML::EndMap;
		out << YAML::EndMap;
	}
	if (auto pointLight = actor->GetComponent<PointLight>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "PointLight";
		out << YAML::BeginMap;
		out << YAML::Key << "Color" << YAML::Value << pointLight->GetColor();
		out << YAML::EndMap;
		out << YAML::EndMap;
	}
	if (auto spotLight = actor->GetComponent<SpotLight>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "SpotLight";
		out << YAML::BeginMap;
		out << YAML::Key << "InnerCutOff" << YAML::Value << spotLight->GetInnerCutOff();
		out << YAML::Key << "OuterCutOff" << YAML::Value << spotLight->GetOuterCutOff();
		out << YAML::Key << "Color" << YAML::Value << spotLight->GetColor();
		out << YAML::EndMap;
		out << YAML::EndMap;
	}
	if (auto skyLight = actor->GetComponent<SkyLight>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "SkyLight";
		out << YAML::BeginMap;

		out << YAML::Key << "Paths" << YAML::Value << YAML::BeginMap;
		for (uint16_t i = 0; i < skyLight->GetPaths().size(); i++)
		{
			out << YAML::Key << i << YAML::Value << skyLight->GetPaths()[i];
		}
		out << YAML::EndMap;

		out << YAML::Key << "Color" << YAML::Value << skyLight->m_Color;
		out << YAML::Key << "Intensity" << YAML::Value << skyLight->m_Intensity;

		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto particleSystem = actor->GetComponent<ParticleSystemComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "ParticleSystem";
		out << YAML::BeginMap;
		out << YAML::Key << "ParticlesCount" << YAML::Value << particleSystem->m_ParticlesCount;
		out << YAML::Key << "SphereRadius" << YAML::Value << particleSystem->m_Radius;
		out << YAML::Key << "MinVelocity" << YAML::Value << particleSystem->m_MinVelocity;
		out << YAML::Key << "MaxVelocity" << YAML::Value << particleSystem->m_MaxVelocity;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto camera = actor->GetComponent<CameraComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Camera";
		out << YAML::BeginMap;
		out << YAML::Key << "AspectRatio" << YAML::Value << camera->m_AspectRatio;
		out << YAML::Key << "FieldOfView" << YAML::Value << camera->m_FieldOfView;
		out << YAML::Key << "NearClipPlane" << YAML::Value << camera->m_NearClipPlane;
		out << YAML::Key << "FarClipPlane" << YAML::Value << camera->m_FarClipPlane;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto boxCollider = actor->GetComponent<BoxColliderComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "BoxCollider";
		out << YAML::BeginMap;
		out << YAML::Key << "IsTrigger" << YAML::Value << boxCollider->m_IsTrigger;
		out << YAML::Key << "Center" << YAML::Value << boxCollider->m_Center;
		out << YAML::Key << "Size" << YAML::Value << boxCollider->m_Size;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto sphereCollider = actor->GetComponent<SphereColliderComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "SphereCollider";
		out << YAML::BeginMap;
		out << YAML::Key << "IsTrigger" << YAML::Value << sphereCollider->m_IsTrigger;
		out << YAML::Key << "Center" << YAML::Value << sphereCollider->m_Center;
		out << YAML::Key << "Size" << YAML::Value << sphereCollider->m_Size;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto rigidBody = actor->GetComponent<RigidBodyComponent>())
    {
        out << YAML::BeginMap;
        out << YAML::Key << "RigidBody";
        out << YAML::BeginMap;
        out << YAML::Key << "Gravity" << YAML::Value << rigidBody->m_Gravity;
        out << YAML::Key << "Mass" << YAML::Value << rigidBody->m_Mass;
        out << YAML::Key << "Drag" << YAML::Value << rigidBody->m_Drag;
        out << YAML::EndMap;
        out << YAML::EndMap;
    }
	
	if (auto image = actor->GetComponent<ImageComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Image";
		out << YAML::BeginMap;
		out << YAML::Key << "Path" << YAML::Value << image->m_Image->GetPath();
		out << YAML::Key << "Color" << YAML::Value << image->m_Color;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto button = actor->GetComponent<ButtonComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "UIButton";
		out << YAML::BeginMap;
		out << YAML::Key << "Enabled" << YAML::Value << button->m_Enabled;
		out << YAML::Key << "ImagePath" << YAML::Value << button->m_Image->GetPath();
		out << YAML::Key << "NormalColor" << YAML::Value << button->m_NormalColor;
		out << YAML::Key << "HoveredColor" << YAML::Value << button->m_HoveredColor;
		out << YAML::Key << "PressedColor" << YAML::Value << button->m_PressedColor;
		out << YAML::Key << "DisabledColor" << YAML::Value << button->m_DisabledColor;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	/* --- GAME COMPONENTS --- */

	if (auto menu = actor->GetComponent<MainMenu>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "MainMenu";
		out << YAML::BeginMap;
		out << YAML::Key << "PlayButton" << YAML::Value << menu->m_PlayButton->GetOwner()->GetID();
		out << YAML::Key << "OptionsButton" << YAML::Value << menu->m_OptionsButton->GetOwner()->GetID();
		out << YAML::Key << "ExitButton" << YAML::Value << menu->m_ExitButton->GetOwner()->GetID();
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto player = actor->GetComponent<Player>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Player";
		out << YAML::BeginMap;
		out << YAML::Key << "Camera" << YAML::Value << player->m_Camera->GetOwner()->GetID();
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto button = actor->GetComponent<Button>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Button";
		out << YAML::BeginMap;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	out << YAML::EndSeq;
	out << YAML::EndMap;
}
