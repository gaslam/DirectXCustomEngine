#pragma once
#include "GameObject.h"

namespace Engine
{
	class Scene
	{
	public:
		explicit Scene(const std::wstring& name) : m_Name{name}
		{
			
		}

		virtual ~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) noexcept = delete;
		bool operator==(Scene& other) = delete;
		bool operator==(Scene&& other) noexcept = delete;

		void RootRender();
		void RootInitialize();
		void RootUpdate(float deltaTime);
		virtual void OnSceneDeactivated() {};
		virtual void OnSceneActivated() {};
		std::wstring GetName() const { return m_Name; }

	protected:
		virtual void Initialize() = 0;
		virtual void Render(){};
		virtual void PostRender(){};
		virtual void Update(float /*deltaTime*/){};

		void AddChild(GameObject* object);
		void RemoveChild(GameObject* object);
	private:
		const std::wstring m_Name{};

		std::vector<std::unique_ptr<GameObject>> m_pChildren{};
	};
}


