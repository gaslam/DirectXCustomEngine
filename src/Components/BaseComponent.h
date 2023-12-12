#pragma once
#include "Logger.h"
#include "Structs/Contexts.h"

namespace Engine
{
	class GameObject;
	class BaseComponent
	{
	public:
		BaseComponent() = default;
		virtual ~BaseComponent() = default;
		virtual void OnScreensizeChanged(const int, const int){};

		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) noexcept = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) noexcept = delete;

		virtual void Initialize(const SceneContext& context) = 0;
		virtual void PostInitialize() {}
		virtual void Update(const SceneContext&) {}
		virtual void Render(const SceneContext&) {}
		virtual void ShadowMapRender() {}
		virtual void PostRender() {}
		virtual void OnOwnerAttach(GameObject* /*pOwner*/) {}
		virtual void OnOwnerDetach(GameObject* /*pObject*/) {}
		virtual void OnSceneAttach() {}
		virtual void OnSceneDetach() {}
		virtual void OnDeviceLost(){}
		void SetOwner(GameObject* pOwner)
		{
			m_pOwner = pOwner;
		}
	private:
		GameObject* m_pOwner{};
	protected:
		GameObject* GetOwner() const { return m_pOwner; }

	};
}
