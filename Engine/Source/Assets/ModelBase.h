#pragma once

#include "Core.h"

class ModelBase
{
public:
	ModelBase(const std::string& path);

	inline const std::string& GetPath() const { return m_Path; }

private:
	std::string m_Path;
};