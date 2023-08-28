#pragma once
#include "Logger.h"
namespace Engine
{
	class GameObject;
	class BaseComponent
	{
	public:
		BaseComponent() = default;
		virtual ~BaseComponent() = default;

		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) noexcept = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) noexcept = delete;

		virtual void Initialize() = 0;
		virtual void PostInitialize() {}
		virtual void Update(float /*elapsedTime*/) {}
		virtual void Render() {}
		virtual void ShadowMapRender() {}
		virtual void PostRender() {}
		virtual void OnOwnerAttach(GameObject* /*pOwner*/) {}
		virtual void OnOwnerDetach(GameObject* /*pObject*/) {}
		virtual void OnSceneAttach() {}
		virtual void OnSceneDetach() {}
		void SetOwner(GameObject* pOwner)
		{
			if(!pOwner)
			{
				const auto pLogger{ Logger::GetInstance() };
				pLogger->LogError(L"Owner cannot be a nullptr!!");
			}
			m_pOwner = pOwner;
		}
	private:
		GameObject* m_pOwner{};
	protected:
		GameObject* GetOwner() const { return m_pOwner; }

	};
}
