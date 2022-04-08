#pragma once

#include <vector>

template <typename T>
class Container
{
public:
	inline void Add(T object) { m_Objects.push_back(object); }
	inline std::vector<T> Get() const { return m_Objects; }
	inline size_t Count() const { return m_Objects.size(); }
	inline T AtIndex(size_t index) const { return m_Objects.at(index); }

private:
	std::vector<T> m_Objects;
};