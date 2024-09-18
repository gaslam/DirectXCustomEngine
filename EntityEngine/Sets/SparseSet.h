#pragma once
#include <iostream>
#include <vector>

namespace EntityEngine {


	constexpr auto SPARSE_INVALID_ID = -1;


	template <std::integral T>
	class SparseSet
	{
		std::vector<T> m_Sparse{};
		std::vector<T> m_Dense{};
		T m_Size;
		T m_Capacity;
		T m_Max;

	public:
		SparseSet(T cap, T maxV)
		{

			m_Sparse.resize(maxV);
			m_Dense.resize(cap);
			m_Size = 0;
			m_Capacity = cap;
			m_Max = maxV;
		}

		void Reserve(T max)
		{
			m_Dense.reserve(max);
			m_Max = max;
		}

		[[nodiscard]] bool Contains(T x) const
		{
			return x > 0 && x <= m_Max && (m_Sparse[x] < m_Size && m_Dense[m_Sparse[x]] == x);
		}

		bool Remove(T x)
		{
			if (!Contains(x)) return false;

			--m_Size;
			m_Dense[m_Sparse[x]] = SPARSE_INVALID_ID;

			return true;
		}

		void Insert(T x)
		{
			if (x > m_Max) return;
			if (m_Size >= m_Capacity) return;
			if (Contains(x)) return;

			m_Dense[m_Size] = x;
			m_Sparse[x] = m_Size;
			++m_Size;
		}

		[[nodiscard]] T Size() const
		{
			return m_Size;
		}

		[[nodiscard]] T Capacity() const
		{
			return m_Capacity;
		}

		[[nodiscard]] T Max() const
		{
			return m_Max;
		}

		void Clear()
		{
			std::fill(m_Sparse.begin(), m_Sparse.end(), SPARSE_INVALID_ID);
			std::fill(m_Dense.begin(), m_Dense.end(), SPARSE_INVALID_ID);
			m_Size = 0;
		}
	};
}