#include "Mesh.h"

#include <glad/glad.h>

Mesh::Mesh(std::vector<uint32_t> indices)
	: m_Indices(indices)
{
}

void Mesh::CreateBounds(std::vector<Vertex> vertices)
{
	std::vector<glm::vec3> points;
	for (auto vertex : vertices)
		points.push_back(vertex.Position);

	m_BoundingBox = BoundingBox(points);
	m_BoundingSphere = BoundingSphere(points);
}

void Mesh::Render()
{
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}