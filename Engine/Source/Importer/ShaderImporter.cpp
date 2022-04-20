#include "ShaderImporter.h"

ShaderImporter::ShaderImporter()
{
}

Ref<Shader> ShaderImporter::ImportShader(const std::string& path)
{
	if (m_ImportedShaders.find(path) != m_ImportedShaders.end())
		return m_ImportedShaders.at(path);

	std::string nameWithExt = path.substr(path.find_last_of("/") + 1);
	std::string name = nameWithExt.substr(0, nameWithExt.find_last_of("."));

	ShaderSource source = ParseShader(path);
	Ref<Shader> shader = Shader::Create(name, source.VertexSource, source.FragmentSource, source.GeometrySource);

	m_ImportedShaders.insert({ path, shader });
	return shader;
}

ShaderSource ShaderImporter::ParseShader(const std::string& path)
{
	enum class ShaderType { Vertex, Fragment, Geometry };

	std::ifstream filestream;
	std::stringstream ss[3];
	filestream.exceptions(std::ifstream::badbit);
	try
	{
		filestream.open(path);

		std::string line;
		ShaderType type;
		while (getline(filestream, line))
		{
			if (line.find("#SHADER") != std::string::npos)
			{
				if (line.find("VERTEX") != std::string::npos)
					type = ShaderType::Vertex;
				else if (line.find("FRAGMENT") != std::string::npos)
					type = ShaderType::Fragment;
				else if (line.find("GEOMETRY") != std::string::npos)
					type = ShaderType::Geometry;
			}
			else
				ss[(int)type] << line << '\n';
		}

		filestream.close();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Reading shader from path " << path << " failed! Exception: " << e.what() << std::endl;
	}

	

	

	return { ss[0].str(), ss[1].str(), ss[2].str() };
}
