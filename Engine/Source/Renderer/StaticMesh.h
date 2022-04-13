#pragma once

#include "Mesh.h"

#include "Core.h"

class StaticMesh : public Mesh
{
public:
	StaticMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

	inline std::vector<Vertex> GetVertices() const { return m_Vertices; }

protected:
	virtual void SetupMesh() override;

private:
	std::vector<Vertex> m_Vertices;
};