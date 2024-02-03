#pragma once
#include "GameObject.h"
#include "Components/CameraComponent.h"

namespace Engine
{
	class Scene
	{
	public:
		explicit Scene(const std::wstring& name) : m_Name{std::move(name)}
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
		void RootUpdate();
		virtual void OnSceneDeactivated() {};
		virtual void OnSceneActivated() {};
		void RootOnDeviceLost();
		virtual void OnDeviceLost() {};
		[[nodiscard]]std::wstring GetName() const { return m_Name; }
		[[nodiscard]] const CameraComponent* GetActiveCamera() const { return m_pActiveCamera; }

	protected:
		virtual void Initialize();
		virtual void Render(){};
		virtual void PostRender(){};
		virtual void RenderImGui() {};
		virtual void Update(){};

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

		std::vector<std::unique_ptr<GameObject>> m_pChildren{};
		bool m_ImGuiVisible{ true };
	};
}


