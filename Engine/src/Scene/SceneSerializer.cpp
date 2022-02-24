#include "SceneSerializer.h"

#include "yaml/yaml.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/InstanceRenderedMeshComponent.h"
#include "Scene/Component/Light/DirectionalLight.h"
#include "Scene/Component/Light/PointLight.h"
#include "Scene/Component/Light/SpotLight.h"
#include "Scene/Component/Light/SkyLight.h"
#include "Scene/Component/ParticleSystemComponent.h"
#include "Scene/Component/PlayerComponent.h"

void SceneSerializer::Serialize(Ref<Scene> scene)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "Untitled";
	out << YAML::Key << "Camera";
	out << YAML::BeginMap;
	out << YAML::Key << "Position" << YAML::Value << scene->m_Camera->Position;
	out << YAML::Key << "Yaw" << YAML::Value << scene->m_Camera->Yaw;
	out << YAML::Key << "Pitch" << YAML::Value << scene->m_Camera->Pitch;
	out << YAML::Key << "Movement Speed" << YAML::Value << scene->m_Camera->MovementSpeed;
	out << YAML::EndMap;
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
	for (auto entity : scene->GetEntities())
	{
		SerializeEntity(out, entity);
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream file("../../res/scenes/Showcase.scene");
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
	
	if (YAML::Node camera = data["Camera"])
	{
		glm::vec3 cameraPosition = camera["Position"].as<glm::vec3>();
		float cameraYaw = camera["Yaw"].as<float>();
		float cameraPitch = camera["Pitch"].as<float>();
		float cameraSpeed = camera["Movement Speed"].as<float>();

		scene->m_Camera->Position = cameraPosition;
		scene->m_Camera->Yaw = cameraYaw;
		scene->m_Camera->Pitch = cameraPitch;
		scene->m_Camera->MovementSpeed = cameraSpeed;
	}

	YAML::Node entities = data["Entities"];
	if (entities)
	{
		auto parentsIDs = std::vector<uint64_t>();

		for (auto entity : entities)
		{
			Ref<Entity> e = Ref<Entity>();
			if (entity["ID"].as<uint64_t>() == 0)
			{
				e = scene->AddRoot();
			}
			else
			{
				if (!scene->GetRoot())
				{
					std::cout << "Loaded scene doesn't contain root entity!" << std::endl;
				}

				e = scene->AddEntity(entity["ID"].as<uint64_t>(), entity["Entity"].as<std::string>());
			}
			
			e->SetID(entity["ID"].as<uint64_t>());
			
			if (auto parent = entity["Parent"])
			{
				parentsIDs.push_back(parent.as<uint64_t>());
			}
			else
			{
				parentsIDs.push_back(-1);
			}

			auto transform = entity["Transform"];
			e->SetLocalPosition(transform["Position"].as<glm::vec3>());
			e->SetLocalRotation(transform["Rotation"].as<glm::vec3>());
			e->SetLocalScale(transform["Scale"].as<glm::vec3>());

			if (auto mesh = entity["Model"])
			{
				std::string path = mesh["Mesh"].as<std::string>();
				std::vector<std::string> materialsPaths;
				YAML::Node materials = mesh["Materials"];
				for (auto material : materials)
				{
					materialsPaths.push_back(material["Path"].as<std::string>());

				}
				e->AddComponent<StaticMeshComponent>(path.c_str(), materialsPaths);
			}

			if (auto mesh = entity["Instance Rendered Mesh"])
			{
				std::string path = mesh["Mesh"].as<std::string>();
				std::vector<std::string> materialsPaths;
				YAML::Node materials = mesh["Materials"];
				for (auto material : materials)
				{
					materialsPaths.push_back(material["Path"].as<std::string>());
				}

				float radius = mesh["Radius"].as<float>();
				int count = mesh["Instances Count"].as<int>();
				float minScale = mesh["Min Mesh Scale"].as<float>();
				float maxScale = mesh["Max Mesh Scale"].as<float>();

				auto m = e->AddComponent<InstanceRenderedMeshComponent>(path.c_str(), materialsPaths);
				m->m_Radius = radius;
				m->m_InstancesCount = count;
				m->m_MinMeshScale = minScale;
				m->m_MaxMeshScale = maxScale;
				m->Generate();
			}

			if (auto dirLight = entity["Directional Light"])
			{
				glm::vec3 color = dirLight["Color"].as<glm::vec3>();

				auto l = e->AddComponent<DirectionalLight>(scene->m_LightsVertexUniformBuffer, scene->m_LightsFragmentUniformBuffer);
				l->SetColor(color);
			}

			if (auto pointLight = entity["Point Light"])
			{
				glm::vec3 color = pointLight["Color"].as<glm::vec3>();

				auto l = e->AddComponent<PointLight>(scene->m_LightsVertexUniformBuffer, scene->m_LightsFragmentUniformBuffer);
				l->SetColor(color);
			}

			if (auto spotLight = entity["Spot Light"])
			{
				float innerCutOff = spotLight["Inner Cut Off"].as<float>();
				float outerCutOff = spotLight["Outer Cut Off"].as<float>();
				glm::vec3 color = spotLight["Color"].as<glm::vec3>();

				auto l = e->AddComponent<SpotLight>(scene->m_LightsVertexUniformBuffer, scene->m_LightsFragmentUniformBuffer);
				l->SetInnerCutOff(innerCutOff);
				l->SetOuterCutOff(outerCutOff);
				l->SetColor(color);
			}

			if (auto skyLight = entity["Sky Light"])
			{
				std::string path = skyLight["Path"].as<std::string>();

				auto l = e->AddComponent<SkyLight>(path);
			}

			if (auto particle = entity["Particle System"])
			{
				int count = particle["Particles Count"].as<int>();
				float radius = particle["Sphere Radius"].as<float>();
				glm::vec3 minVelocity = particle["Min Velocity"].as<glm::vec3>();
				glm::vec3 maxVelocity = particle["Max Velocity"].as<glm::vec3>();

				auto p = e->AddComponent<ParticleSystemComponent>();
				p->m_ParticlesCount = count;
				p->m_Radius = radius;
				p->m_MinVelocity = minVelocity;
				p->m_MaxVelocity = maxVelocity;

				p->Reset();
			}

			if (auto player = entity["Player"])
			{
				e->AddComponent<PlayerComponent>();
			}
		}

		for (int i = 0; i < parentsIDs.size(); i++)
		{
			if (parentsIDs[i] == -1)
				continue;

			scene->GetEntities()[i]->SetParent(scene->FindEntity(parentsIDs[i]).get());
		}
	}

	file.close();
	return scene;
}

void SceneSerializer::SerializeEntity(YAML::Emitter& out, Ref<Entity> entity)
{
	out << YAML::BeginMap;
	out << YAML::Key << "Entity" << YAML::Value << entity->GetName();
	out << YAML::Key << "ID" << YAML::Value << entity->GetID();
	if (entity->GetParent())
		out << YAML::Key << "Parent" << YAML::Value << entity->GetParent()->GetID();

	Transform transform = entity->GetTransform();
	out << YAML::Key << "Transform";
	out << YAML::BeginMap;
	out << YAML::Key << "Position" << YAML::Value << transform.LocalPosition;
	out << YAML::Key << "Rotation" << YAML::Value << transform.LocalRotation;
	out << YAML::Key << "Scale" << YAML::Value << transform.LocalScale;
	out << YAML::EndMap;

	if (auto mesh = entity->GetComponent<StaticMeshComponent>())
	{
		out << YAML::Key << "Model";
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
	}
	if (auto mesh = entity->GetComponent<InstanceRenderedMeshComponent>())
	{
		out << YAML::Key << "Instance Rendered Mesh";
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
		out << YAML::Key << "Instances Count" << YAML::Value << mesh->m_InstancesCount;
		out << YAML::Key << "Min Mesh Scale" << YAML::Value << mesh->m_MinMeshScale;
		out << YAML::Key << "Max Mesh Scale" << YAML::Value << mesh->m_MaxMeshScale;


		out << YAML::EndMap;
	}
	if (auto dirLight = entity->GetComponent<DirectionalLight>())
	{
		out << YAML::Key << "Directional Light";
		out << YAML::BeginMap;
		out << YAML::Key << "Color" << YAML::Value << dirLight->GetColor();
		out << YAML::EndMap;
	}
	if (auto pointLight = entity->GetComponent<PointLight>())
	{
		out << YAML::Key << "Point Light";
		out << YAML::BeginMap;
		out << YAML::Key << "Color" << YAML::Value << pointLight->GetColor();
		out << YAML::EndMap;
	}
	if (auto spotLight = entity->GetComponent<SpotLight>())
	{
		out << YAML::Key << "Spot Light";
		out << YAML::BeginMap;
		out << YAML::Key << "Inner Cut Off" << YAML::Value << spotLight->GetInnerCutOff();
		out << YAML::Key << "Outer Cut Off" << YAML::Value << spotLight->GetOuterCutOff();
		out << YAML::Key << "Color" << YAML::Value << spotLight->GetColor();
		out << YAML::EndMap;
	}
	if (auto skyLight = entity->GetComponent<SkyLight>())
	{
		out << YAML::Key << "Sky Light";
		out << YAML::BeginMap;
		out << YAML::Key << "Path" << YAML::Value << skyLight->GetPath();
		out << YAML::EndMap;
	}

	if (auto particleSystem = entity->GetComponent<ParticleSystemComponent>())
	{
		out << YAML::Key << "Particle System";
		out << YAML::BeginMap;
		out << YAML::Key << "Particles Count" << YAML::Value << particleSystem->m_ParticlesCount;
		out << YAML::Key << "Sphere Radius" << YAML::Value << particleSystem->m_Radius;
		out << YAML::Key << "Min Velocity" << YAML::Value << particleSystem->m_MinVelocity;
		out << YAML::Key << "Max Velocity" << YAML::Value << particleSystem->m_MaxVelocity;
		out << YAML::EndMap;
	}

	if (auto player = entity->GetComponent<PlayerComponent>())
	{
		out << YAML::Key << "Player";
		out << YAML::BeginMap;
		out << YAML::EndMap;
	}

	out << YAML::EndMap;
}
