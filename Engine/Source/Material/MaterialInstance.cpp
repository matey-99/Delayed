#include "MaterialInstance.h"

MaterialInstance::MaterialInstance(Ref<Material> material)
	: Material(material->GetName() + " Instance", material->GetPath(), material->GetShader())
{
	SetBlendMode(material->GetBlendMode());

	m_BoolParameters = material->GetBoolParameters();
	m_FloatParameters = material->GetFloatParameters();
	m_Vec3Parameters = material->GetVec3Parameters();
	m_Texture2DParameters = material->GetTexture2DParameters();
}

Ref<MaterialInstance> MaterialInstance::Create(Ref<Material> material)
{
	return CreateRef<MaterialInstance>(material);
}

void MaterialInstance::SetBoolParameter(std::string name, bool value)
{
	if (m_BoolParameters.find(name) != m_BoolParameters.end())
		m_BoolParameters.find(name)->second = value;
}

void MaterialInstance::SetFloatParameter(std::string name, float value)
{
	if (m_FloatParameters.find(name) != m_FloatParameters.end())
		m_FloatParameters.find(name)->second = value;
}

void MaterialInstance::SetVec3Parameter(std::string name, glm::vec3 value)
{
	if (m_Vec3Parameters.find(name) != m_Vec3Parameters.end())
		m_Vec3Parameters.find(name)->second = value;
}

void MaterialInstance::SetTexture2DParameter(std::string name, Ref<Texture> value)
{
	if (m_Texture2DParameters.find(name) != m_Texture2DParameters.end())
		m_Texture2DParameters.find(name)->second = value;
}
