#pragma once

#include "Core.h"

#include "Renderer/Shader.h"

class ShaderImporter
{
public:
	ShaderImporter();

	Ref<Shader> ImportShader(const std::string& path);

private:
	ShaderSource ParseShader(const std::string& path);

private:
	std::unordered_map<std::string, Ref<Shader>> m_ImportedShaders;
};