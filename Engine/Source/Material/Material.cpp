#include "Material.h"

#include "MaterialSerializer.h"
#include "Importer/MaterialImporter.h"
#include "ShaderLibrary.h"
#include "Assets/AssetManager.h"

Material::Material(std::string name, std::string path, Ref<Shader> shader)
	: m_Name(name), m_Path(path), m_Shader(shader)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution distribution(1, 999999999);

	m_ID = distribution(gen);
	m_BlendMode = BlendMode::Opaque;

	m_BoolParameters.clear();
	m_FloatParameters.clear();
	m_Vec3Parameters.clear();
	m_Vec4Parameters.clear();
	m_Texture2DParameters.clear();

	LoadParameters();
}

Material::Material(uint64_t id, std::string name, std::string path, Ref<Shader> shader)
	: m_ID(id), m_Name(name), m_Path(path), m_Shader(shader)
{
	m_BlendMode = BlendMode::Opaque;

	LoadParameters();
}

Ref<Material> Material::Create(std::string name, std::string path, Ref<Shader> shader)
{
	return CreateRef<Material>(name, path, shader);
}

Ref<Material> Material::Create(std::string name, std::string path, std::string shaderName)
{
	return CreateRef<Material>(name, path, ShaderLibrary::GetInstance()->GetShader(ShaderType::Material, shaderName));
}

Ref<Material> Material::Create(uint64_t id, std::string name, std::string path, std::string shaderName)
{
	return CreateRef<Material>(id, name, path, ShaderLibrary::GetInstance()->GetShader(ShaderType::Material, shaderName));
}

void Material::Use()
{
	m_Shader->Use();

	for (auto& param : m_BoolParameters)
	{
		m_Shader->SetBool(param.first, param.second);
	}
	for (auto& param : m_FloatParameters)
	{
		m_Shader->SetFloat(param.first, param.second);
	}
	for (auto& param : m_Vec3Parameters)
	{
		m_Shader->SetVec3(param.first, param.second);
	}
	for (auto& param : m_Vec4Parameters)
	{
		m_Shader->SetVec4(param.first, param.second);
	}

	int index = 0;
	for (auto& param : m_Texture2DParameters)
	{
		if (param.second)
		{
			param.second->Bind(index);
			m_Shader->SetInt(param.first, index);
			index++;
		}
	}
}

void Material::LoadParameters()
{
	std::vector<std::string> parameters;
	std::vector<ShaderUniform> uniforms = m_Shader->GetUniforms();
	for (auto uniform : uniforms)
	{
		std::string uniformName = uniform.Name.substr(0, uniform.Name.find_first_of('.'));
		if (uniformName == "u_Material" || uniformName == "u_MaterialVS")
		{
			//std::string name = uniform.name.substr(uniform.name.find_first_of('.') + 1);

			switch (uniform.Type)
			{
			case ShaderUniformType::BOOL:
				if (m_BoolParameters.find(uniformName) == m_BoolParameters.end())
				{
					m_BoolParameters.insert({ uniform.Name, false });
					parameters.push_back(uniform.Name);
				}
				break;
			case ShaderUniformType::INT:
				break;
			case ShaderUniformType::FLOAT:
				if (m_FloatParameters.find(uniformName) == m_FloatParameters.end())
				{
					m_FloatParameters.insert({ uniform.Name, 0.0 });
					parameters.push_back(uniform.Name);
				}
				break;
			case ShaderUniformType::VEC3:
				if (m_Vec3Parameters.find(uniformName) == m_Vec3Parameters.end())
				{
					m_Vec3Parameters.insert({ uniform.Name, glm::vec3(0.0f) });
					parameters.push_back(uniform.Name);
				}
				break;
			case ShaderUniformType::VEC4:
				if (m_Vec4Parameters.find(uniformName) == m_Vec4Parameters.end())
				{
					m_Vec4Parameters.insert({ uniform.Name, glm::vec4(0.0f) });
					parameters.push_back(uniform.Name);
				}
				break;
			case ShaderUniformType::SAMPLER_2D:
				if (m_Texture2DParameters.find(uniformName) == m_Texture2DParameters.end())
				{
					m_Texture2DParameters.insert({ uniform.Name, Ref<Texture>() });
					parameters.push_back(uniform.Name);
				}
				break;
			}
		}
	}

	for (auto& param : m_BoolParameters)
	{
		if (std::find(parameters.begin(), parameters.end(), param.first) == parameters.end())
		{
			m_BoolParameters.erase(param.first);
			break;
		}
	}

	for (auto& param : m_FloatParameters)
	{
		if (std::find(parameters.begin(), parameters.end(), param.first) == parameters.end())
		{
			m_FloatParameters.erase(param.first);
			break;
		}
	}

	for (auto& param : m_Vec3Parameters)
	{
		if (std::find(parameters.begin(), parameters.end(), param.first) == parameters.end())
		{
			m_Vec3Parameters.erase(param.first);
			break;
		}
	}

	for (auto& param : m_Vec4Parameters)
	{
		if (std::find(parameters.begin(), parameters.end(), param.first) == parameters.end())
		{
			m_Vec4Parameters.erase(param.first);
			break;
		}
	}

	for (auto& param : m_Texture2DParameters)
	{
		if (std::find(parameters.begin(), parameters.end(), param.first) == parameters.end())
		{
			m_Texture2DParameters.erase(param.first);
			break;
		}
	}
}

bool Material::GetBoolParameter(std::string name)
{
	if (m_BoolParameters.find(name) != m_BoolParameters.end())
		return m_BoolParameters.find(name)->second;

	return false;
}

float Material::GetFloatParameter(std::string name)
{
	if (m_FloatParameters.find(name) != m_FloatParameters.end())
		return m_FloatParameters.find(name)->second;

	return 0.0f;
}

glm::vec3 Material::GetVec3Parameter(std::string name)
{
	if (m_Vec3Parameters.find(name) != m_Vec3Parameters.end())
		return m_Vec3Parameters.find(name)->second;

	return glm::vec3();
}

glm::vec4 Material::GetVec4Parameter(std::string name)
{
	if (m_Vec4Parameters.find(name) != m_Vec4Parameters.end())
		return m_Vec4Parameters.find(name)->second;

	return glm::vec4();
}

Ref<Texture> Material::GetTexture2DParameter(std::string name)
{
	if (m_Texture2DParameters.find(name) != m_Texture2DParameters.end())
		return m_Texture2DParameters.find(name)->second;

	return nullptr;
}