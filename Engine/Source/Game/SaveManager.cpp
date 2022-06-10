#include "SaveManager.h"

#include "Saveable.h"
#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "yaml/yaml.h"

SaveManager* SaveManager::s_Instance{};

SaveManager::SaveManager(Actor* owner)
	: GameComponent(owner)
{
}

void SaveManager::Start()
{
	if (!s_Instance)
		s_Instance = this;

	GetAllSaveables();

	if (!LoadGame())
		SaveGame();
}

void SaveManager::SaveGame()
{
	YAML::Emitter out;
	out << YAML::BeginMap;

	std::time_t saveTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string saveTimeStr = std::string(std::ctime(&saveTime));
	std::string saveTimeStrFormatted = saveTimeStr.substr(0, saveTimeStr.size() - 1);

	out << YAML::Key << "GameSave" << YAML::Value << saveTimeStrFormatted;

	out << YAML::Key << "Instances" << YAML::Value << YAML::BeginSeq;
	for (auto& saveable : m_Saveables)
	{
		SaveData data = saveable->Save();

		out << YAML::BeginMap;
		out << YAML::Key << "ActorID" << YAML::Value << data.ActorID;

		out << YAML::Key << "BoolFields" << YAML::Value << YAML::BeginSeq;
		for (auto& it : data.BoolFields)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Name" << YAML::Value << it.first;
			out << YAML::Key << "Value" << YAML::Value << it.second;
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;

		out << YAML::Key << "IntFields" << YAML::Value << YAML::BeginSeq;
		for (auto& it : data.IntFields)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Name" << YAML::Value << it.first;
			out << YAML::Key << "Value" << YAML::Value << it.second;
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;

		out << YAML::Key << "FloatFields" << YAML::Value << YAML::BeginSeq;
		for (auto& it : data.FloatFields)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Name" << YAML::Value << it.first;
			out << YAML::Key << "Value" << YAML::Value << it.second;
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;

		out << YAML::Key << "StringFields" << YAML::Value << YAML::BeginSeq;
		for (auto& it : data.StringFields)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Name" << YAML::Value << it.first;
			out << YAML::Key << "Value" << YAML::Value << it.second;
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;

		out << YAML::Key << "Vector3Fields" << YAML::Value << YAML::BeginSeq;
		for (auto& it : data.Vector3Fields)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Name" << YAML::Value << it.first;
			out << YAML::Key << "Value" << YAML::Value << it.second;
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;

		out << YAML::EndMap;
	}
	
	out << YAML::EndMap;

	if (!std::filesystem::is_directory("Saves"))
		std::filesystem::create_directory("Saves");

	std::ofstream file("Saves/Save.yaml");
	file << out.c_str();
	file.close();
}

bool SaveManager::LoadGame()
{
	if (!std::filesystem::is_directory("Saves"))
		return false;

	std::ifstream file("Saves/Save.yaml");
	std::stringstream ss;
	ss << file.rdbuf();

	YAML::Node data = YAML::Load(ss.str());
	if (!data["GameSave"])
		return false;

	auto scene = m_Owner->GetScene();

	YAML::Node instances = data["Instances"];
	if (instances)
	{
		for (auto instance : instances)
		{
			auto actorID = instance["ActorID"].as<uint64_t>();
			auto actor = scene->FindActor(actorID);
			if (actor)
			{
				for (auto& comp : actor->GetComponents())
				{
					if (auto saveable = Cast<Saveable>(comp))
					{
						SaveData saveData;

						YAML::Node boolFields = instance["BoolFields"];
						if (boolFields)
						{
							for (auto field : boolFields)
							{
								std::string name = field["Name"].as<std::string>();
								bool value = field["Value"].as<bool>();

								saveData.BoolFields.insert({ name, value });
							}
						}

						YAML::Node intFields = instance["IntFields"];
						if (intFields)
						{
							for (auto field : intFields)
							{
								std::string name = field["Name"].as<std::string>();
								int value = field["Value"].as<int>();

								saveData.IntFields.insert({ name, value });
							}
						}

						YAML::Node floatFields = instance["FloatFields"];
						if (floatFields)
						{
							for (auto field : floatFields)
							{
								std::string name = field["Name"].as<std::string>();
								float value = field["Value"].as<float>();

								saveData.FloatFields.insert({ name, value });
							}
						}

						YAML::Node stringFields = instance["StringFields"];
						if (stringFields)
						{
							for (auto field : stringFields)
							{
								std::string name = field["Name"].as<std::string>();
								std::string value = field["Value"].as<std::string>();

								saveData.StringFields.insert({ name, value });
							}
						}

						YAML::Node vec3Fields = instance["Vector3Fields"];
						if (vec3Fields)
						{
							for (auto field : vec3Fields)
							{
								std::string name = field["Name"].as<std::string>();
								glm::vec3 value = field["Value"].as<glm::vec3>();

								saveData.Vector3Fields.insert({ name, value });
							}
						}

						saveable->Load(saveData);
					}
				}
			}
		}
	}

	file.close();
	return true;
}

void SaveManager::GetAllSaveables()
{
	m_Saveables.clear();

	for (auto& gameComponent : m_Owner->GetScene()->GetComponents<GameComponent>())
	{
		if (auto saveable = Cast<Saveable>(gameComponent))
			m_Saveables.push_back(saveable);
	}
}

bool SaveManager::IsSaveAvailable() {
    if (!std::filesystem::is_directory("Saves"))
        return false;

    std::ifstream file("Saves/Save.yaml");
    std::stringstream ss;
    ss << file.rdbuf();

    YAML::Node data = YAML::Load(ss.str());
    if (!data["GameSave"])
        return false;

    return true;
}