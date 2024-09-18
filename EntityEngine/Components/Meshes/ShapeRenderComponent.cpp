#include "pch.h"
#include "ShapeRenderComponent.h"

#include "Managers/TextureManager.h"
#include "SceneClasses/Scene.h"

void ShapeRenderComponent::InitDeviceResources()
{
	MeshRenderComponent::InitDeviceResources();
	const GameHandlerBase* pHandler{ Locator::GetGameHandler() };
	DeviceResources* pDeviceResources{ pHandler->GetDeviceResources() };
	ID3D12Device* pDevice{ pHandler->GetDevice() };

	m_pResourceDescriptors = std::make_unique<DescriptorHeap>(pDevice,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		Descriptors::Count);

	ResourceUploadBatch resourceUpload(pDevice);

	auto pTexture{ EntityEngine::TextureManager::GetInstance()->GetTexture(L"earth.bmp") };

	resourceUpload.Begin();

		CreateShaderResourceView(pDevice, pTexture.Get(), m_pResourceDescriptors->GetCpuHandle(Descriptors::Earth));

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

	for (unique_ptr<BasicEffect>& pEffect : m_pEffects)
	{
		pEffect.reset();
	}
}

void ShapeRenderComponent::SetShape(std::unique_ptr<GeometricPrimitive>& shape)
{
	m_pShape = std::move(shape);
}

void ShapeRenderComponent::Initialize(Scene* pScene)
{

	pScene->AddRenderCallback([&](const Scene* pScene) -> void
		{

			const CameraComponent* pCamera{ pScene->GetActiveCamera() };

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
		});
}
