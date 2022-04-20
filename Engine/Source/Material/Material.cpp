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
	m_BoolParameters.clear();
	m_FloatParameters.clear();
	m_Vec3Parameters.clear();
	m_Texture2DParameters.clear();

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
				m_BoolParameters.insert({ uniform.Name, false });
				break;
			case ShaderUniformType::INT:
				break;
			case ShaderUniformType::FLOAT:
				m_FloatParameters.insert({ uniform.Name, 0.0 });
				break;
			case ShaderUniformType::VEC3:
				m_Vec3Parameters.insert({ uniform.Name, glm::vec3(0.0f) });
				break;
			case ShaderUniformType::SAMPLER_2D:
				m_Texture2DParameters.insert({ uniform.Name, Ref<Texture>() });
			}
		}
	}
}

