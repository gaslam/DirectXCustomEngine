#pragma once
#include "Scene.h"

namespace Engine
{
	class TestScene :
		public Scene
	{
	public:

		TestScene() : Scene(L"TestScene") {}
		~TestScene() override = default;

		void Initialize() override {};
		void Render() override {};
		void PostRender() override {};
		void Update(float /*deltaTime*/) override {};
	};
}


