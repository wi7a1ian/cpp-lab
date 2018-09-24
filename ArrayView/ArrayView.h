#pragma once

#include <utility>
#include <algorithm>

template <class T>
class ArrayView
{
public:
	typedef const T * Ptr;

private:
	Ptr m_pData;
	size_t m_length;

public:
	// Default constructor
	ArrayView() :
		m_pData(nullptr),
		m_length(0)
	{
	}

	// Constructor
	ArrayView(const Ptr pData, const size_t length) :
		m_pData(pData),
		m_length(length)
	{
	}

	// Iterators
	Ptr begin() const { return m_pData; }
	Ptr end() const { return m_pData + m_length; }
	Ptr cbegin() const { return m_pData; }
	Ptr cend() const { return m_pData + m_length; }

	// Capacity
	bool empty() const { return m_length == 0; }
	size_t size() const { return m_length; }
	void clear() { *this = ArrayView(); }

	// Element access
	T operator [] (const size_t pos) const { return *(m_pData + pos); }
	Ptr data() const { return m_pData; }

	// Modifiers
	void swap(ArrayView &other) { std::swap(*this, other); }

	// Relational operators
	bool operator < (const ArrayView &other) const { return compare(other) < 0; }
	bool operator > (const ArrayView &other) const { return other < *this; }
	bool operator <= (const ArrayView &other) const { return !(*this > other); }
	bool operator >= (const ArrayView &other) const { return !(*this < other); }
	bool operator == (const ArrayView &other) const { return compare(other) == 0; }
	bool operator != (const ArrayView &other) const { return !(*this == other); }

	int compare(const ArrayView &other) const
	{
		const size_t minLength = std::min(m_length, other.m_length);
		for (size_t i = 0; i < minLength; ++i)
		{
			if (m_pData[i] < other.m_pData[i])
				return -1;

			if (m_pData[i] > other.m_pData[i])
				return 1;
		}

		if (m_length < other.m_length)
			return -1;

		if (m_length > other.m_length)
			return 1;

		return 0;
	}
};