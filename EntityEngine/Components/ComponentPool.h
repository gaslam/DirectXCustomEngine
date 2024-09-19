#pragma once
#include "Sets/SparseSet.h"

namespace EntityEngine
{
	using Entity = unsigned int;

	class IComponentPool
	{
	public:
		IComponentPool() = default;
		virtual ~IComponentPool() = default;
	};



	template <typename T>
	class ComponentPool final : public IComponentPool
	{
		SparseSet<Entity> m_Entities{ 100000,100000 };
		std::vector<T> m_Components{};

	public:
		T& AddComponent(Entity entity)
		{
			m_Entities.Insert(entity);
			return m_Components.emplace_back(T{});
		}


		template <typename... Args>
		T& AddComponent(Entity entity, Args&&... args)
		{
			m_Entities.Insert(entity);
			return m_Components.emplace_back(T{ std::forward<Args>(args)... });
		}

		bool Contains(Entity entity) const
		{
			return m_Entities.Contains(entity);
		}

		void Remove(Entity entity)
		{
			m_Entities.Remove(entity);
		}

		[[nodiscard]] const T& GetComponent(Entity entity) const
		{
			return m_Components[entity];
		}

		[[nodiscard]] T& GetComponent(Entity entity)
		{
			return m_Components[entity];
		}

		[[nodiscard]] size_t GetComponentCount() const { return m_Components.size(); }

		void Clear()
		{
			m_Entities.Clear();
			m_Components.clear();
		}


	};
}

