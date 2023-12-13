#include "pch.h"
#include "MeshRenderComponent.h"

#include "CameraComponent.h"
#include "SceneClasses/GameObject.h"

using namespace DirectX;

void MeshRenderComponent::Initialize(const SceneContext& context)
{

    GameObject* owner{ GetOwner() };

    m_pTransform = owner->GetTransform();
    if(!m_pTransform)
    {
        Logger::LogWarning(L"Cannot translate the object cause no transform was found");
        return;
    }

    DX::DeviceResources* pDeviceResources{ context.d12Context.pDeviceResources };
    ID3D12Device* pDevice{ pDeviceResources->GetD3DDevice() };
    m_resourceDescriptors = std::make_unique<DescriptorHeap>(pDevice, 1);

    m_states = std::make_unique<CommonStates>(pDevice);

    RenderTargetState rtState(pDeviceResources->GetBackBufferFormat(),
        pDeviceResources->GetDepthBufferFormat());

    {
        EffectPipelineStateDescription pd(
            &GeometricPrimitive::VertexType::InputLayout,
            CommonStates::Opaque,
            CommonStates::DepthDefault,
            CommonStates::CullClockwise,
            rtState);

        m_Effect = std::make_unique<BasicEffect>(pDevice,
            EffectFlags::Lighting, pd);
        m_Effect->EnableDefaultLighting();
    }

    ResourceUploadBatch resourceUpload(pDevice);

    resourceUpload.Begin();

    const future<void> uploadResourcesFinished{ resourceUpload.End(
        pDeviceResources->GetCommandQueue()) };
    uploadResourcesFinished.wait();

    pDeviceResources->WaitForGpu();
}

void MeshRenderComponent::Render(const SceneContext& context)
{

    if(!m_pTransform)
    {
        return;
    }
    const auto pCamera{ context.pCameraComp };
    const Matrix view{ pCamera->GetViewMatrix() };
    const Matrix proj{ pCamera->GetProjectionMatrix() };
    const auto commandList{ context.d12Context.pDeviceResources->GetCommandList() };
    ID3D12DescriptorHeap* heaps[] = {
    m_resourceDescriptors->Heap(), m_states->Heap()
    };
    commandList->SetDescriptorHeaps(std::size(heaps),
        heaps);

    const auto world{ m_pTransform->GetWorldMatrix() };
    m_Effect->SetMatrices(world, view, proj);
    m_Effect->Apply(commandList);
    m_Shape->Draw(commandList);
}

void MeshRenderComponent::OnDeviceLost()
{
    m_Shape.reset();
    m_roomTex.Reset();
    m_resourceDescriptors.reset();
    m_states.reset();
    m_Effect.reset();
}

void MeshRenderComponent::SetShape(std::unique_ptr<GeometricPrimitive>& shape)
{
    m_Shape = std::move(shape);
}
