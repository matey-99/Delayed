#pragma once

#include "typedefs.h"

class UniformBuffer
{
private:
	uint32_t m_ID;

public:
	UniformBuffer(uint32_t size, uint32_t binding);

	void Bind();
	void Unbind();

	void SetUniform(uint32_t offset, uint32_t size, const void* data);
};