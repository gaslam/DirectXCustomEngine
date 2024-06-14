#pragma once
#include "pch.h"

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

		virtual void Initialize() = 0;
		virtual void PostInitialize() {}
		virtual void InitDeviceResources(){}
		virtual void Update() {}
		virtual void Render() {}
		virtual void ShadowMapRender() {}
		virtual void PostRender() {}
		virtual void OnEntityAttach(GameObject* /*pOwner*/) {}
		virtual void OnEntityDetach(GameObject* /*pObject*/) {}
		virtual void OnSceneAttach() {}
		virtual void OnSceneDetach() {}
		virtual void OnDeviceLost(){}
		virtual void RenderImGui(){}
		void SetOwner(GameObject* pOwner)
		{
			m_pOwner = pOwner;
		}
	private:
		GameObject* m_pOwner{};
	protected:
		[[nodiscard]]GameObject* GetOwner() const
		{
			if(!m_pOwner)
			{
				Logger::LogError(L"Cannot find component owner!!");
			}
			return m_pOwner;
		}

	};
}
