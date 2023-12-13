#pragma once
#include "SceneClasses/Scene.h"

class Circle;

namespace Engine
{
	class TestScene :
		public Scene
	{
	public:

		TestScene() : Scene(L"TestScene") {}
		~TestScene() override = default;

		void Initialize() override;
		void Render() override {};
		void PostRender() override {};
		void Update(const SceneContext& context) override;
	private:
		Circle* m_Circle{};

		float m_MinScale{ 1 };
		float m_ScaleSpeed{ 1.f };
		float m_MaxScale{ 5 };
	};
}


