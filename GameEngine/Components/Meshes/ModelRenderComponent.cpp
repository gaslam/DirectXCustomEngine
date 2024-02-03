#include "pch.h"
#include "ModelRenderComponent.h"

#include "Components/CameraComponent.h"
#include "SceneClasses/Scene.h"
#include "Utils/Utils.h"

void ModelRenderComponent::InitDeviceResources()
{
	MeshRenderComponent::InitDeviceResources();
	const std::wstring folderLocation{ GetFolderLocation() };

	if (folderLocation.empty())
	{
		return;
	}

	const GameHandlerBase* pHandler{ Locator::GetGameHandler() };
	ID3D12Device* pDevice{ pHandler->GetDevice() };
	const DeviceResources* pDeviceResources{ pHandler->GetDeviceResources() };
	const CommonStates* pStates{ GetStates() };


	m_pModel = Model::CreateFromSDKMESH(pDevice, folderLocation.c_str());

	ResourceUploadBatch resourceUpload(pDevice);
	const std::wstring texturePath{ pHandler->GetContentRoot() + L"Textures/" };

	resourceUpload.Begin();
	m_pModelResources = m_pModel->LoadTextures(pDevice, resourceUpload, texturePath.c_str());
	m_pFxFactory = std::make_unique<EffectFactory>(m_pModelResources->Heap(), pStates->Heap());

	const future<void> uploadResourcesFinished{ resourceUpload.End(
		pDeviceResources->GetCommandQueue()) };

	uploadResourcesFinished.wait();


	const RenderTargetState rtState(pDeviceResources->GetBackBufferFormat(),
		pDeviceResources->GetDepthBufferFormat());

	EffectPipelineStateDescription pd(
		nullptr,
		CommonStates::Opaque,
		CommonStates::DepthDefault,
		CommonStates::CullClockwise,
		rtState);


	m_ModelNormal = m_pModel->CreateEffects(*m_pFxFactory, pd, pd);
}

void ModelRenderComponent::Render()
{
	const GameHandlerBase* pHandler{ Locator::GetGameHandler() };
	const DeviceResources* pDeviceResources{ pHandler->GetDeviceResources() };
	const auto pCommandList{ pDeviceResources->GetCommandList() };
	const CommonStates* pStates{ GetStates() };

	const GameObject* pOwner{ GetOwner() };
	if (!pOwner)
	{
		Logger::LogError(L"Cannot get component owner!!");
		return;
	}

	const Scene* pParentScene{ pOwner->GetParentScene() };
	if (!pParentScene)
	{
		Logger::LogError(L"Cannot get object parent scene!!");
		return;
	}

	const TransformComponent* pTransform{ GetTransform() };
	if (!pTransform)
	{
		Logger::LogWarning(L"Cannot transform object!!");
		return;
	}

	const CameraComponent* pCamera{ pParentScene->GetActiveCamera() };

	const Matrix view{ pCamera->GetViewMatrix() };
	const Matrix proj{ pCamera->GetProjectionMatrix() };
	const Matrix world{ pTransform->GetWorldMatrix() };

	ID3D12DescriptorHeap* heaps[] = { m_pModelResources->Heap(), pStates->Heap() };
	pCommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

	Model::UpdateEffectMatrices(m_ModelNormal, world, view, proj);

	m_pModel->Draw(pCommandList, m_ModelNormal.cbegin());
}

void ModelRenderComponent::OnDeviceLost()
{
	m_pStates.reset();
	m_pModelResources.reset();
	m_pFxFactory.reset();
	m_pModel.reset();;
}
