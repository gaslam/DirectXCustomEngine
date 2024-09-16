#include "pch.h"
#include "MeshRenderComponent.h"
#include "SceneClasses/GameObject.h"
#include "Utils/GameHandler.h"

using namespace DirectX;

void MeshRenderComponent::Initialize(Scene*)
{
	const GameObject* owner{ GetOwner() };

	m_pTransform = owner->GetTransform();
	if (!m_pTransform)
	{
		Logger::LogWarning(L"Cannot translate the object cause no transform was found");
	}
}

void MeshRenderComponent::InitDeviceResources()
{
	const GameHandlerBase* pHandler{ Locator::GetGameHandler() };
	ID3D12Device* pDevice{ pHandler->GetDevice() };
	m_pStates = std::make_unique<CommonStates>(pDevice);
}

void MeshRenderComponent::OnDeviceLost()
{
	m_pStates.reset();
}

std::wstring MeshRenderComponent::GetFolderLocation() const
{
	const GameHandlerBase* pHandler{ Locator::GetGameHandler() };
	const std::wstring folder{ pHandler->GetContentRoot() };
	const std::wstring result{   folder + L"Textures/" + m_FolderLocation };
	return result;
}
