#include "MaterialSerializer.h"

#include "Assets/AssetManager.h"

#include "yaml/yaml.h"

void MaterialSerializer::Serialize(Ref<Material> material, std::string destinationPath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Material" << YAML::Value << material->GetName();
	out << YAML::Key << "ID" << YAML::Value << material->GetID();
	out << YAML::Key << "BlendMode" << YAML::Value << (uint16_t)material->GetBlendMode();
	out << YAML::Key << "Shader" << YAML::Value << material->GetShader()->GetName();

	out << YAML::Key << "Bool Parameters" << YAML::Value << YAML::BeginSeq;
	for (auto param : material->m_BoolParameters)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << param.first;
		out << YAML::Key << "Value" << YAML::Value << param.second;
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;

	out << YAML::Key << "Float Parameters" << YAML::Value << YAML::BeginSeq;
	for (auto param : material->m_FloatParameters)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << param.first;
		out << YAML::Key << "Value" << YAML::Value << param.second;
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;

	out << YAML::Key << "Vec3 Parameters" << YAML::Value << YAML::BeginSeq;
	for (auto param : material->m_Vec3Parameters)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << param.first;
		out << YAML::Key << "Value" << YAML::Value << param.second;
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;

	out << YAML::Key << "Texture Parameters" << YAML::Value << YAML::BeginSeq;
	for (auto param : material->m_Texture2DParameters)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << param.first;

		if (param.second)
		{
			out << YAML::Key << "Path" << YAML::Value << param.second->GetPath();
		}
		else
			out << YAML::Key << "Path" << YAML::Value << "null";

		out << YAML::EndMap;
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream file(destinationPath);
	file << out.c_str();
	file.close();
}

Ref<Material> MaterialSerializer::Deserialize(std::string path)
{
	std::ifstream file(path);
	std::stringstream ss;
	ss << file.rdbuf();

	YAML::Node data = YAML::Load(ss.str());
	if (!data["Material"])
	{
		std::cout << "Cannot load material from path: " << path << std::endl;
		return Ref<Material>();
	}

	std::string name = data["Material"].as<std::string>();
	uint64_t id = data["ID"].as<uint64_t>();
	Material::BlendMode blend = (Material::BlendMode)data["BlendMode"].as<uint16_t>();
	std::string shader = data["Shader"].as<std::string>();

	std::string materialPath = path.substr(AssetManager::ContentDirectory.size());

	Ref<Material> material = Material::Create(id, name, materialPath, shader);
	material->m_BlendMode = blend;

	YAML::Node boolParameters = data["Bool Parameters"];
	if (boolParameters)
	{
		for (auto param : boolParameters)
		{
			std::string name = param["Name"].as<std::string>();
			bool value = param["Value"].as<bool>();

			if (material->m_BoolParameters.find(name) != material->m_BoolParameters.end())
				material->m_BoolParameters.find(name)->second = value;
		}
	}

	YAML::Node floatParameters = data["Float Parameters"];
	if (floatParameters)
	{
		for (auto param : floatParameters)
		{
			std::string name = param["Name"].as<std::string>();
			float value = param["Value"].as<float>();
			
			if (material->m_FloatParameters.find(name) != material->m_FloatParameters.end())
				material->m_FloatParameters.find(name)->second = value;
		}
	}

	YAML::Node vec3Parameters = data["Vec3 Parameters"];
	if (vec3Parameters)
	{
		for (auto param : vec3Parameters)
		{
			std::string name = param["Name"].as<std::string>();
			glm::vec3 value = param["Value"].as<glm::vec3>();

			if (material->m_Vec3Parameters.find(name) != material->m_Vec3Parameters.end())
				material->m_Vec3Parameters.find(name)->second = value;

		}
	}

	YAML::Node textureParameters = data["Texture Parameters"];
	if (textureParameters)
	{
		for (auto param : textureParameters)
		{
			std::string name = param["Name"].as<std::string>();
			std::string path = param["Path"].as<std::string>();

			if (path != "null")
			{
				Ref<Texture> texture = AssetManager::LoadTexture(path);

				if (material->m_Texture2DParameters.find(name) != material->m_Texture2DParameters.end())
					material->m_Texture2DParameters.find(name)->second = texture;
			}
		}
	}

	file.close();
	return material;
}
