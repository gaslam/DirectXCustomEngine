#pragma once
#include <typeindex>
#include <unordered_map>
#include <memory>
#include <ranges>
#include <vector>

#include "imgui.h"
#include "Components/BaseComponent.h"
#include "Components/TransformComponent.h"
#include "Utils/Utils.h"

using namespace std;
namespace Engine
{
	class Scene;

	class GameObject
	{
	public:

		GameObject()
		{
			m_pTransform = AddComponent<TransformComponent>();
			m_Tag = L"gameObject";
		};

		explicit GameObject(std::wstring& tag)
		{
			m_pTransform = AddComponent<TransformComponent>();
			m_Tag = tag;
		}
		virtual ~GameObject() = default;

		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) noexcept = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject& operator=(GameObject&&) noexcept = delete;

		template <typename T>
		enable_if_t<is_base_of_v<BaseComponent, T>, bool> IsComponentAdded()
		{
			const type_index typeIndex{ type_index(typeid(T)) };
			return m_pComponents.contains(typeIndex);
		}

		template <typename  T>
		enable_if_t<is_base_of_v<BaseComponent, T>, T*> GetComponent()
		{
			const type_index typeIndex{ type_index(typeid(T)) };
			const auto it{ m_pComponents.find(typeIndex) };
			if (it == m_pComponents.end())
			{
				return nullptr;
			}
			return dynamic_cast<T*>(m_pComponents[typeIndex].get());
		}

		template <typename T, typename... Args>
		enable_if_t<is_base_of_v<BaseComponent, T>, T*>AddComponent(Args&&... args)
		{
			if (IsComponentAdded<T>())
			{
				return GetComponent<T>();
			}
			const type_index typeIndex{ type_index(typeid(T)) };
			auto pComponent{ make_unique<T>(forward<Args>(args)...) };
			pComponent->SetOwner(this);
			pComponent->OnEntityAttach(this);
			auto pointer{ pComponent.get() };
			m_pComponents.emplace(typeIndex, move(pComponent));
			return pointer;

		}

		template <typename T>
		void RemoveComponent()
		{
			const type_index typeIndex = type_index(typeid(T));
			auto component{ dynamic_cast<T*>(m_pComponents.at(typeIndex).get()) };
			component->OnEntityDetach(this);
			if (component)
			{
				m_pComponents.erase(typeIndex);
			}
		}

		void RemoveChild(GameObject* child)
		{
			const auto foundObject = std::ranges::find_if(m_pChildren.begin(), m_pChildren.end(), [child](const std::unique_ptr<GameObject>& obj)
				{
					return obj.get() == child;
				});
			if (foundObject != m_pChildren.end())
			{
				m_pChildren.erase(foundObject);
			}
		}

		template
			<typename T>
			enable_if_t<is_base_of_v<GameObject, T>, T*> AddChild(T* child)
		{
			child->m_pParent = this;
			child->RootSceneAttach(m_pParentScene);
			m_pChildren.emplace_back(child);
			return child;
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

		void Render() const
		{
			for (const auto& val : m_pComponents | views::values)
			{
				val->Render();
			}
			for (const auto& pChild : m_pChildren)
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

		void RootRenderImGui()
		{
			ImGui::PushID(this);
			const std::string tag{ StringUtils::DecodeUTF8(m_Tag.c_str()) };
			if (ImGui::CollapsingHeader(tag.c_str()))
			{
				RenderImGui();
				for (const auto& val : m_pComponents | views::values)
				{
					val->RenderImGui();
				}
				for (const auto& pChild : m_pChildren)
				{
					pChild->RootRenderImGui();
				}
			}
			ImGui::PopID();

		}

		void ChangeTag(const std::wstring& tag)
		{
			m_Tag = tag;
		}

		[[nodiscard]] Scene* GetParentScene() const { return m_pParentScene; }
		[[nodiscard]] GameObject* GetParent() const { return m_pParent; }
		[[nodiscard]] TransformComponent* GetTransform() const { return m_pTransform; }
		[[nodiscard]] TransformComponent* GetTag() const { return m_pTransform; }

	protected:
		void RootInitialize(Scene* pScene)
		{
			if (m_Initialized)
			{
				return;
			}
			Initialize(pScene);
			for (const auto& pComponent : m_pComponents | views::values)
			{
				pComponent->Initialize();
			}

			for (const auto& pChild : m_pChildren)
			{
				pChild->RootInitialize(pScene);
			}

			m_Initialized = true;
		}

		void RootInitDeviceResources()
		{
			if (m_DeviceRcInitialized)
			{
				return;
			}
			InitDeviceResources();
			for (const auto& pComponent : m_pComponents | views::values)
			{
				pComponent->InitDeviceResources();
			}

			for (const auto& pChild : m_pChildren)
			{
				pChild->RootInitDeviceResources();
			}

			m_DeviceRcInitialized = true;
		}

		virtual void Initialize(Scene*) {};
		virtual void InitDeviceResources() {};

		virtual void RootOnScreensizeChanged(const int width, const int height)
		{
			OnScreensizeChanged(width, height);
			for (const auto& pComponent : m_pComponents | views::values)
			{
				pComponent->OnScreensizeChanged(width, height);
			}

			for (const auto& pChild : m_pChildren)
			{
				pChild->RootOnScreensizeChanged(width, height);
			}
		};

		virtual void OnScreensizeChanged(const int, const int)
		{

		}

		virtual void RenderImGui() {};

		virtual void OnSceneAttach(Scene* /*pScene*/) {}

	private:
		friend class Scene;
		void RootSceneAttach(Scene* pScene);

		bool m_Initialized{ false };
		bool m_DeviceRcInitialized{ false };
		std::wstring m_Tag{};

		GameObject* m_pParent{};
		Scene* m_pParentScene{};

		vector<std::unique_ptr<GameObject>> m_pChildren{};
		unordered_map<type_index, unique_ptr<BaseComponent>> m_pComponents{};
		TransformComponent* m_pTransform{};
	};
}