#pragma once
#include "Components/BaseComponent.h"
#include <CommonStates.h>

using namespace DirectX;

using namespace Engine;
class TransformComponent;
class MeshRenderComponent : public BaseComponent
{
public:
	explicit MeshRenderComponent(const std::wstring& dir = L"") : m_FolderLocation{ dir } {};
	virtual ~MeshRenderComponent() override = default;
	virtual void Initialize(Scene* pScene) override;
	virtual void InitDeviceResources() override;
	virtual void OnDeviceLost() override;


	[[nodiscard]] std::wstring GetFolderLocation() const;
	[[nodiscard]] CommonStates* GetStates() const { return m_pStates.get(); }
	[[nodiscard]] TransformComponent* GetTransform() const { return m_pTransform; }
private:
	const std::wstring m_FolderLocation{};
	std::unique_ptr<CommonStates> m_pStates;


	TransformComponent* m_pTransform{};
};

