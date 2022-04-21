#include "ComputeShaderImporter.h"

ComputeShaderImporter::ComputeShaderImporter()
{
}


Ref<ComputeShader> ComputeShaderImporter::ImportComputeShader(const std::string& path)
{
	if (m_ImportedComputeShaders.find(path) != m_ImportedComputeShaders.end())
		return m_ImportedComputeShaders.at(path);

	std::string nameWithExt = path.substr(path.find_last_of("/") + 1);
	std::string name = nameWithExt.substr(0, nameWithExt.find_last_of("."));

	std::string source = ParseShader(path);
	Ref<ComputeShader> computeShader = ComputeShader::Create(name, source);

	m_ImportedComputeShaders.insert({ path, computeShader });
	return computeShader;
}

std::string ComputeShaderImporter::ParseShader(const std::string& path)
{
    std::string source;
    std::ifstream filestream;

    filestream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        std::stringstream buffer;

        filestream.open(path);
        buffer << filestream.rdbuf();
        source = buffer.str();
        filestream.close();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "Reading shader from path: " << path << " failed!" << std::endl;
    }

    return source;
}

