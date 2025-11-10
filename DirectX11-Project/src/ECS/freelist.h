#pragma once

#include <vector>

#define INVALID_INDEX (-1)

template<typename T>
class FreeList
{
public:
	FreeList()
		: m_firstFree(INVALID_INDEX)
	{
	}
	FreeList(const FreeList& other) = default;
	~FreeList() = default;

	T& operator[](unsigned int index)
	{
		return m_elements[index].data;
	}

	template<typename... Args>
	unsigned int Emplace(Args&&... args)
	{
		unsigned int index = INVALID_INDEX;
		if (m_firstFree == INVALID_INDEX) {
			m_elements.emplace_back(std::forward<Args>(args)...);
			index = static_cast<unsigned int>(m_elements.size()) - 1;
		}
		else {
			index = m_firstFree;
			m_firstFree = m_elements[m_firstFree].next;
			m_elements[m_firstFree].data = T(std::forward<Args>(args)...);
		}
		return index;
	}

	void Remove(unsigned int index)
	{
		m_elements[index].next = m_firstFree;
		m_firstFree = index;
	}

private:
	union ListElement
	{
		unsigned int next;
		T data;
	};

private:
	unsigned int m_firstFree;
	std::vector<ListElement> m_elements;
};

#undef INVALID_INDEX