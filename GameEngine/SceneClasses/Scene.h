#pragma once
#include "GameObject.h"
#include "Components/CameraComponent.h"

namespace Engine
{
	class Scene
	{
	public:
		explicit Scene(const std::wstring& name) : m_Name{name}
		{
		}

		virtual ~Scene() = default;
		void RootOnScreensizeChanged(const int width, const int height);
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) noexcept = delete;
		bool operator==(Scene& other) = delete;
		bool operator==(Scene&& other) noexcept = delete;

		void RootRender();
		void RootRenderImGui() const;
		void RootInitialize();
		void RootInitDeviceResources() const;
		virtual void InitDeviceResources(){};
		void Update() const
		{
			for (const auto& callback : m_pUpdateCallbacks)
			{
				callback(this);
			}

			for (const auto& callback : m_pFixedUpdateCallbacks)
			{
				callback(this);
			}
		};
		virtual void OnSceneDeactivated() {};
		virtual void OnSceneActivated() {};
		void RootOnDeviceLost();
		virtual void OnDeviceLost() {};
		[[nodiscard]]std::wstring GetName() const { return m_Name; }
		[[nodiscard]] const CameraComponent* GetActiveCamera() const { return m_pActiveCamera; }

		void AddInitializerCallback(const std::function<void(const Scene*)>& callback) { m_pInitializerCallbacks.emplace_back(callback); }
		void AddUpdateCallback(const std::function<void(const Scene*)>& callback) { m_pUpdateCallbacks.emplace_back(callback); }
		void AddFixedUpdateCallback(const std::function<void(const Scene*)>& callback) { m_pFixedUpdateCallbacks.emplace_back(callback); }

	protected:
		virtual void Initialize();
		virtual void Render(){};
		virtual void PostRender(){};
		virtual void RenderImGui() {};

		template
		<typename Object>
		std::enable_if_t<std::is_base_of_v<GameObject,Object>,Object*> AddChild(Object* object) noexcept
		{
			std::unique_ptr<GameObject> pObject{ std::unique_ptr<GameObject>(object) };
			pObject->RootSceneAttach(this);
			m_pChildren.emplace_back(move(pObject));
			return object;
		}

	private:
		const std::wstring m_Name{};

		CameraComponent* m_pActiveCamera{};
		CameraComponent* m_pDefaultCamera{};

		std::vector<std::function<void(const Scene*)>> m_pInitializerCallbacks{};
		std::vector<std::function<void(const Scene*)>> m_pUpdateCallbacks{};
		std::vector<std::function<void(const Scene*)>> m_pFixedUpdateCallbacks{};
		std::vector<std::unique_ptr<GameObject>> m_pChildren{};
		bool m_ImGuiVisible{ true };
	};
}


