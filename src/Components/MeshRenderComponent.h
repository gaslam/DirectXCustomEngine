#pragma once
#include "Components/BaseComponent.h"

using namespace DirectX;

using namespace Engine;
class TransformComponent;
class MeshRenderComponent final : public BaseComponent
{
public:
	explicit MeshRenderComponent(const std::wstring& dir = L"") : m_FolderLocation{ dir } {};
	void Initialize(const SceneContext& context) override;
	void Render(const SceneContext& context) override;
	void OnDeviceLost() override;
	void SetShape(std::unique_ptr<GeometricPrimitive>& shape);

private:
	const std::wstring m_FolderLocation{};
	Microsoft::WRL::ComPtr<ID3D12Resource> m_pTexture;
	std::unique_ptr<DescriptorHeap> m_pResourceDescriptors;
	std::unique_ptr<CommonStates> m_pStates;
	std::unique_ptr<BasicEffect> m_pEffect;

	std::unique_ptr<GeometricPrimitive> m_pShape{};

	TransformComponent* m_pTransform{};

	enum Descriptors
	{
		Earth,
		Count
	};
};

