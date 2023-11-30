#pragma once
#include "SceneClasses/Scene.h"

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
		void Update(const SceneContext& /*context*/) override;
	private:
		std::unique_ptr<DirectX::GeometricPrimitive> m_room;

		DirectX::SimpleMath::Color m_roomColor;

		Microsoft::WRL::ComPtr<ID3D12Resource> m_roomTex;
		std::unique_ptr<DirectX::DescriptorHeap> m_resourceDescriptors;
		std::unique_ptr<DirectX::CommonStates> m_states;
		std::unique_ptr<DirectX::BasicEffect> m_roomEffect;
	};
}


