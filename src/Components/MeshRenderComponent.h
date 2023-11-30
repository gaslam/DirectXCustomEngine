#pragma once
#include "Components/BaseComponent.h"

using namespace DirectX;

namespace
{
	const XMVECTORF32 START_POSITION = { 0.f, -1.5f, 0.f, 0.f };
	const XMVECTORF32 ROOM_BOUNDS = { 8.f, 6.f, 12.f, 0.f };
	constexpr float ROTATION_GAIN = 0.1f;
}

using namespace Engine;
class MeshRenderComponent final : public BaseComponent
{
public:
	MeshRenderComponent() = default;
	void Initialize(const SceneContext& context) override;
	void Render(const SceneContext& context) override;
	void OnDeviceLost() override;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_roomTex;
	std::unique_ptr<DescriptorHeap> m_resourceDescriptors;
	std::unique_ptr<CommonStates> m_states;
	std::unique_ptr<BasicEffect> m_roomEffect;

	std::unique_ptr<DirectX::GeometricPrimitive> m_room;
	DirectX::SimpleMath::Vector3 m_cameraPos;
	DirectX::SimpleMath::Color m_RoomColor;

};

