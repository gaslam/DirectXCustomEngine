#pragma once
#include "Prefabs/Shapes/Circle.h"
#include "SceneClasses/Scene.h"

namespace Exe
{
	class TestScene final: public Engine::Scene
	{
	public:
		explicit TestScene(const std::wstring& name) : Scene(name)
		{
		}

		~TestScene() override = default;
		TestScene(const TestScene& other) = delete;
		TestScene(TestScene&& other) noexcept = delete;
		TestScene& operator=(const TestScene& other) = delete;
		TestScene& operator=(TestScene&& other) noexcept = delete;

		void Initialize() override;

	private:
		Circle* m_pCircle{};
	};
}