#include "pch.h"
#include "ShapeRenderComponent.h"

#include "SceneClasses/Scene.h"


void ShapeRenderComponent::Render()
{
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

	const CameraComponent* pCamera{ pParentScene->GetActiveCamera() };

	TransformComponent* pTransform{ GetTransform() };
	if (!pTransform)
	{
		Logger::LogWarning(L"Cannot transform object!!");
		return;
	}
	const auto pHandler{ Locator::GetGameHandler() };

	const Matrix view{ pCamera->GetViewMatrix() };
	const Matrix proj{ pCamera->GetProjectionMatrix() };
	const auto pDeviceResources{ pHandler->GetDeviceResources() };
	const auto commandList{ pDeviceResources->GetCommandList() };
	const CommonStates* pStates{ GetStates() };

	ID3D12DescriptorHeap* heaps[] = {
	m_pResourceDescriptors->Heap(), pStates->Heap()
	};
	commandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)),
		heaps);

	const auto world{ pTransform->GetWorldMatrix() };
	for (const unique_ptr<BasicEffect>& pEffect : m_pEffects)
	{
		pEffect->SetMatrices(world, view, proj);
		pEffect->Apply(commandList);
	}

	m_pShape->Draw(commandList);
}

void ShapeRenderComponent::InitDeviceResources()
{
	MeshRenderComponent::Initialize();
	const GameHandlerBase* pHandler{ Locator::GetGameHandler() };
	DeviceResources* pDeviceResources{ pHandler->GetDeviceResources() };
	ID3D12Device* pDevice{ pHandler->GetDevice() };
	const std::wstring folderLocation{ GetFolderLocation() };
	ResourceUploadBatch resourceUpload(pDevice);

	resourceUpload.Begin();
	if (!folderLocation.empty())
	{

		const HRESULT result{ CreateWICTextureFromFile(pDevice,resourceUpload,folderLocation.c_str(),m_pTexture.ReleaseAndGetAddressOf(),false) };

		if (FAILED(result))
		{
			Logger::LogError(L"Cannot load texture in directory: " + folderLocation);
		}

		CreateShaderResourceView(pDevice, m_pTexture.Get(), m_pResourceDescriptors->GetCpuHandle(Descriptors::Earth));
	}

	RenderTargetState rtState(pDeviceResources->GetBackBufferFormat(),
		pDeviceResources->GetDepthBufferFormat());

	{
		EffectPipelineStateDescription pd(
			&GeometricPrimitive::VertexType::InputLayout,
			CommonStates::Opaque,
			CommonStates::DepthDefault,
			CommonStates::CullCounterClockwise,
			rtState);

		BasicEffect* pEffect{ new BasicEffect{ pDevice,
			EffectFlags::Lighting | EffectFlags::Texture , pd} };
		CommonStates* pStates{ GetStates() };
		pEffect->EnableDefaultLighting();
		pEffect->SetTexture(m_pResourceDescriptors->GetGpuHandle(Descriptors::Earth), pStates->AnisotropicWrap());
		AddEffect(pEffect);
	}

	//Upload the VB/IB directly to the shape for faster performance
	m_pShape->LoadStaticBuffers(pDevice, resourceUpload);

	const future<void> uploadResourcesFinished{ resourceUpload.End(
		pDeviceResources->GetCommandQueue()) };
	uploadResourcesFinished.wait();

	pDeviceResources->WaitForGpu();
}

void ShapeRenderComponent::OnDeviceLost()
{
	MeshRenderComponent::OnDeviceLost();
	m_pShape.reset();
	m_pTexture.Reset();

	for (unique_ptr<BasicEffect>& pEffect : m_pEffects)
	{
		pEffect.reset();
	}
}

void ShapeRenderComponent::SetShape(std::unique_ptr<GeometricPrimitive>& shape)
{
	m_pShape = std::move(shape);
}
