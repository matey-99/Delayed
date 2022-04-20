#include "MeshBase.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

MeshBase::MeshBase(std::vector<uint32_t> indices)
	: m_Indices(indices)
{
}

void MeshBase::CreateBounds(std::vector<Vertex> vertices)
{
	std::vector<glm::vec3> points;
	for (auto vertex : vertices)
		points.push_back(vertex.Position);

	m_BoundingBox = BoundingBox(points);
	m_BoundingSphere = BoundingSphere(points);
}

void MeshBase::Render()
{
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void MeshBase::RenderInstanced(uint32_t instancesCount, std::vector<glm::mat4> modelMatrices)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_TransformationVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instancesCount, &modelMatrices[0], GL_DYNAMIC_DRAW);

	glBindVertexArray(m_VAO);
	glDrawElementsInstanced(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0, instancesCount);
	glBindVertexArray(0);
}
