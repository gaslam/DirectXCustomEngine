#pragma once
#include "GameObject.h"
#include "Structs/Contexts.h"

namespace Engine
{
	class Scene
	{
	public:
		explicit Scene(std::wstring name) : m_Name{std::move(name)}, m_SceneContext{}
		{
			
		}

		virtual ~Scene() = default;
		void RootOnScreensizeChanged(const int width, const int height);
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) noexcept = delete;
		bool operator==(Scene& other) = delete;
		bool operator==(Scene&& other) noexcept = delete;

		void RootRender();
		void RootInitialize(GameContext& context);
		void RootUpdate();
		virtual void OnSceneDeactivated() {};
		virtual void OnSceneActivated() {};
		void RootOnDeviceLost();
		virtual void OnDeviceLost() {};
		std::wstring GetName() const { return m_Name; }
		[[nodiscard]] SceneContext GetSceneContext() const { return m_SceneContext; }

	protected:
		virtual void Initialize();
		virtual void Render(){};
		virtual void PostRender(){};
		virtual void Update(const SceneContext&){};

		GameObject* AddChild(GameObject* object);
		void RemoveChild(GameObject* object);
	private:
		const std::wstring m_Name{};

		CameraComponent* m_pActiveCamera{};
		CameraComponent* m_pDefaultCamera{};

		std::vector<std::unique_ptr<GameObject>> m_pChildren{};

		SceneContext m_SceneContext{};
	};
}


