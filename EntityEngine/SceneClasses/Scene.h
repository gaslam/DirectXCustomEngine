#pragma once
#include "GameObject.h"
#include "ECS/Registry.h"

namespace EntityEngine
{
	class CameraComponentECS;
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
		[[nodiscard]] CameraComponentECS* GetActiveCamera() const { return m_pActiveCamera; }

		void AddInitializerCallback(const std::function<void(const Scene*)>& callback) { m_pInitializerCallbacks.emplace_back(callback); }
		void AddUpdateCallback(const std::function<void(const Scene*)>& callback) { m_pUpdateCallbacks.emplace_back(callback); }
		void AddFixedUpdateCallback(const std::function<void(const Scene*)>& callback) { m_pFixedUpdateCallbacks.emplace_back(callback); }
		void AddRenderCallback(const std::function<void(const Scene*)>& callback) { m_pRenderCallbacks.emplace_back(callback); }
		[[nodiscard]] Registry* GetRegistry() const { return m_pRegistry.get(); }

	protected:
		virtual void Initialize();
		virtual void RenderImGui() {};

	private:
		const std::wstring m_Name{};

		CameraComponentECS* m_pActiveCamera{};

		std::vector<std::function<void(const Scene*)>> m_pInitializerCallbacks{};
		std::vector<std::function<void(const Scene*)>> m_pUpdateCallbacks{};
		std::vector<std::function<void(const Scene*)>> m_pFixedUpdateCallbacks{};
		std::vector<std::function<void(const Scene*)>> m_pRenderCallbacks{};

		std::unique_ptr<Registry> m_pRegistry{ std::make_unique<Registry>() };
		bool m_ImGuiVisible{ true };
	};
}


