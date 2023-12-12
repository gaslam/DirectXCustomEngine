#pragma once
#include <typeindex>
#include <unordered_map>
#include <memory>
#include <ranges>
#include <vector>

#include "../Components/BaseComponent.h"
#include "../Components/TransformComponent.h"

using namespace std;
namespace Engine
{
	class Scene;

	class GameObject
	{
	public:

		GameObject() = default;
		virtual ~GameObject() = default;

		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) noexcept = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject& operator=(GameObject&&) noexcept = delete;

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
			pComponent->SetOwner(this);
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
			const auto foundObject = std::ranges::find_if(m_pChildren.begin(), m_pChildren.end(),[child](const std::unique_ptr<GameObject>& obj)
			{
					return obj.get() == child;
			});
			if (foundObject != m_pChildren.end())
			{
				m_pChildren.erase(foundObject);
			}
		}

		GameObject* AddChild(GameObject* child)
		{
			child->RootSceneAttach(m_pParentScene);
			m_pChildren.emplace_back(child);
			return child;
		}

		void Update(const SceneContext& context) const
		{
			for(const auto& val : m_pComponents | views::values)
			{
				val->Update(context);
			}
			for(const auto& pChild: m_pChildren)
			{
				pChild->Update(context);
			}
		}

		void OnDeviceLost() const
		{
			for (const auto& val : m_pComponents | views::values)
			{
				val->OnDeviceLost();
			}
			for (const auto& pChild : m_pChildren)
			{
				pChild->OnDeviceLost();
			}
		}

		void Render(const SceneContext& context) const
		{
			for(const auto& val : m_pComponents | views::values)
			{
				val->Render(context);
			}
			for(const auto& pChild: m_pChildren)
			{
				pChild->Render(context);
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

		[[nodiscard]]Scene* GetParentScene() const { return m_pParentScene; }
		[[nodiscard]]GameObject* GetParent() const { return m_pParent; }
		[[nodiscard]]TransformComponent* GetTransform() const { return m_pTransform; }

	protected:
		void RootInitialize(const SceneContext& sceneContext)
		{
			GameObject* obj{ AddChild(new GameObject{}) };

			m_pTransform = obj->AddComponent<TransformComponent>();

			Initialize(sceneContext);
			for(const auto& pComponent: m_pComponents| views::values)
			{
				pComponent->Initialize(sceneContext);
			}
		}

		virtual void Initialize(const SceneContext&) {};

		virtual void RootOnScreensizeChanged(const int width, const int height)
		{
			OnScreensizeChanged(width,height);
			for (const auto& pComponent : m_pComponents | views::values)
			{
				pComponent->OnScreensizeChanged(width,height);
			}

			for (const auto& pChild : m_pChildren)
			{
				pChild->OnScreensizeChanged(width, height);
			}
		};
		virtual void OnScreensizeChanged(const int, const int)
		{
			
		}

	private:
		friend class Scene;
		void RootSceneAttach(Scene* pScene);
		GameObject* m_pParent{};
		Scene* m_pParentScene{};

		vector<std::unique_ptr<GameObject>> m_pChildren{};
		unordered_map<type_index, unique_ptr<BaseComponent>> m_pComponents{};
		TransformComponent* m_pTransform{};
	};
}