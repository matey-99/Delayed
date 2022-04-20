#pragma once

#include "Core.h"

#include "Renderer/ComputeShader.h"

class ComputeShaderImporter
{
public:
	ComputeShaderImporter();

	Ref<ComputeShader> ImportComputeShader(const std::string& path);

private:
	std::string ParseShader(const std::string& path);

private:
	std::unordered_map<std::string, Ref<ComputeShader>> m_ImportedComputeShaders;
};