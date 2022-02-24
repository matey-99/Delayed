#include "Material.h"

#include "MaterialSerializer.h"
#include "Importer/MaterialImporter.h"
#include "ShaderLibrary.h"

Material::Material(std::string name, Ref<Shader> shader)
	: m_Name(name), m_Shader(shader)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution distribution(1, 999999999);

	m_ID = distribution(gen);
	LoadParameters();
}

Material::Material(uint64_t id, std::string name, Ref<Shader> shader)
	: m_ID(id), m_Name(name), m_Shader(shader)
{
	LoadParameters();
}

Ref<Material> Material::Create(std::string name, Ref<Shader> shader)
{
	Ref<Material> material = CreateRef<Material>(name, shader);
	MaterialSerializer::Serialize(material);
	MaterialImporter::GetInstance()->AddMaterial("../../res/materials/" + material->GetName() + ".mat", material);

	return material;
}

Ref<Material> Material::Create(std::string name, std::string shaderName)
{
	Ref<Material> material = CreateRef<Material>(name, ShaderLibrary::GetInstance()->GetShader(ShaderType::MATERIAL, shaderName));
	MaterialSerializer::Serialize(material);
	MaterialImporter::GetInstance()->AddMaterial("../../res/materials/" + material->GetName() + ".mat", material);

	return material;
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
		std::string uniformName = uniform.name.substr(0, uniform.name.find_first_of('.'));
		if (uniformName == "u_Material" || uniformName == "u_MaterialVS")
		{
			//std::string name = uniform.name.substr(uniform.name.find_first_of('.') + 1);

			switch (uniform.type)
			{
			case ShaderUniformType::BOOL:
				m_BoolParameters.insert({ uniform.name, false });
				break;
			case ShaderUniformType::INT:
				break;
			case ShaderUniformType::FLOAT:
				m_FloatParameters.insert({ uniform.name, 0.0 });
				break;
			case ShaderUniformType::VEC3:
				m_Vec3Parameters.insert({ uniform.name, glm::vec3(0.0f) });
				break;
			case ShaderUniformType::SAMPLER_2D:
				m_Texture2DParameters.insert({ uniform.name, Ref<Texture>() });
			}
		}
	}
}

