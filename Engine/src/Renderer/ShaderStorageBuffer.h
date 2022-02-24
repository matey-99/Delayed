#pragma once

#include "typedefs.h"

#include <glad/glad.h>

template <class T>
class ShaderStorageBuffer
{
public:
	ShaderStorageBuffer(uint32_t size) : m_Size(size)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size * sizeof(T), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	~ShaderStorageBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void Bind()
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ID);
	}

	void Unbind()
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	T* Map(int32_t access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT)
	{
		return (T*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_Size * sizeof(T), access);
	}

	void Unmap()
	{
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}

	inline uint32_t GetID() const { return m_ID; }

private:
	uint32_t m_ID;
	uint32_t m_Size;
};