#pragma once
#include <typeindex>
#include <unordered_map>
#include <memory>
#include <ranges>

#include "../Components/BaseComponent.h"

using namespace std;
namespace Engine
{
	class GameObject final
	{
	public:
		template <typename T>
		enable_if_t<is_base_of_v<BaseComponent, T>, bool> IsComponentAdded()
		{
			const type_index typeIndex { type_index(typeid(T))};
			return m_pComponents.contains(typeIndex);
		}

		template <typename  T>
		enable_if_t<is_base_of_v<BaseComponent,T>,T*> GetComponent()
		{
			const type_index typeIndex { type_index(typeid(T))};
			const auto it{ m_pComponents.find(typeIndex) };
			if(it == m_pComponents.end())
			{
				return nullptr;
			}
			return dynamic_cast<T*>(m_pComponents[typeIndex].get());
		}

		template <typename T, typename... Args>
		enable_if_t<is_base_of_v<BaseComponent, T>, T*>AddComponent(Args&&... args)
		{
			if(IsComponentAdded<T>())
			{
				return GetComponent<T>();
			}
			const type_index typeIndex { type_index(typeid(T))};
			auto pComponent{ make_unique<T>(forward<Args>(args)...) };
			pComponent->SetOwner(nullptr);
			pComponent->Initialize();
			pComponent->OnOwnerAttach(this);
			auto pointer{ pComponent.get()};
			m_pComponents.emplace(typeIndex, move(pComponent));
			return pointer;

		}

		template <typename T>
		void RemoveComponent()
		{
			const type_index typeIndex = type_index(typeid(T));
			auto component{ dynamic_cast<T*>(m_pComponents.at(typeIndex).get()) };
			component->OnOwnerDetach(this);
			if (component)
			{
				m_pComponents.erase(typeIndex);
			}
		}

		void RemoveChild(GameObject* child)
		{
			const auto foundObject = find(m_pChildren.begin(), m_pChildren.end(), child);
			if (foundObject != m_pChildren.end())
			{
				m_pChildren.erase(foundObject);
			}
		}

		void AddChild(GameObject* child)
		{
			m_pChildren.emplace_back(child);
		}

		void Update(float elapsedTime) const
		{
			for(const auto& val : m_pComponents | views::values)
			{
				val->Update(elapsedTime);
			}
			for(const auto& pChild: m_pChildren)
			{
				pChild->Update(elapsedTime);
			}
		}

		void Render() const
		{
			for(const auto& val : m_pComponents | views::values)
			{
				val->Render();
			}
			for(const auto& pChild: m_pChildren)
			{
				pChild->Render();
			}
		}

		void PostRender() const
		{
			for (const auto& val : m_pComponents | views::values)
			{
				val->PostRender();
			}
			for (const auto& pChild : m_pChildren)
			{
				pChild->PostRender();
			}
		}

	private:
		GameObject* m_pParent{};

		vector<GameObject*> m_pChildren{};
		unordered_map<type_index, unique_ptr<BaseComponent>> m_pComponents{};
	};
}