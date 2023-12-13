#pragma once
#include "Components/BaseComponent.h"

using namespace DirectX;

using namespace Engine;
class TransformComponent;
class MeshRenderComponent final : public BaseComponent
{
public:
	MeshRenderComponent() = default;
	void Initialize(const SceneContext& context) override;
	void Render(const SceneContext& context) override;
	void OnDeviceLost() override;
	void SetShape(std::unique_ptr<GeometricPrimitive>& shape);

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_roomTex;
	std::unique_ptr<DescriptorHeap> m_resourceDescriptors;
	std::unique_ptr<CommonStates> m_states;
	std::unique_ptr<BasicEffect> m_Effect;

	std::unique_ptr<GeometricPrimitive> m_Shape{};

	TransformComponent* m_pTransform{};
};

