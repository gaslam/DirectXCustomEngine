#pragma once
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
		virtual void Draw() {}
		virtual void ShadowMapDraw() {}
		virtual void PostDraw() {}
		virtual void OnOwnerAttach(GameObject* /*pOwner*/) {}
		virtual void OnOwnerDetach(GameObject* /*pObject*/) {}
		virtual void OnSceneAttach() {}
		virtual void OnSceneDetach() {}
		void SetOwner(GameObject* pOwner) { m_pOwner = pOwner; }
	private:
		GameObject* m_pOwner{};
	protected:
		GameObject* GetOwner() const { return m_pOwner; }

	};
}
