#include "UniformBuffer.h"

#include <glad/glad.h>

UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, binding, m_ID, 0, size);
}

void UniformBuffer::Bind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
}

void UniformBuffer::Unbind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::SetUniform(uint32_t offset, uint32_t size, const void* data)
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}
