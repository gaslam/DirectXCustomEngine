#pragma once
#include "SceneClasses/Scene.h"

class ModelObject;

namespace Engine
{
	class TestScene :
		public Scene
	{
	public:

		TestScene() : Scene(L"TestScene") {}
		~TestScene() override = default;

	protected:
		void Initialize() override;
		void Render() override {};
		void PostRender() override {};
		void Update() override;
		void RenderImGui() override;
	private:
		ModelObject* m_Model{};

		float m_RotSpeedDeg{ 20.f };
		Vector3 m_Rotation{};
	};
}


