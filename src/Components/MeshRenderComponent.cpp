#include "pch.h"
#include "MeshRenderComponent.h"

#include "CameraComponent.h"
#include "SceneClasses/GameObject.h"

using namespace DirectX;

void MeshRenderComponent::Initialize(const SceneContext& context)
{
    GameObject* owner{ GetOwner() };

    m_pTransform = owner->GetTransform();
    if (!m_pTransform)
    {
        Logger::LogWarning(L"Cannot translate the object cause no transform was found");
        return;
    }

    DX::DeviceResources* pDeviceResources{ context.d12Context.pDeviceResources };
    ID3D12Device* pDevice{ pDeviceResources->GetD3DDevice() };
    m_pResourceDescriptors = std::make_unique<DescriptorHeap>(pDevice, Descriptors::Count);

    m_pStates = std::make_unique<CommonStates>(pDevice);

    RenderTargetState rtState(pDeviceResources->GetBackBufferFormat(),
        pDeviceResources->GetDepthBufferFormat());

    {
        EffectPipelineStateDescription pd(
            &GeometricPrimitive::VertexType::InputLayout,
            CommonStates::Opaque,
            CommonStates::DepthDefault,
            CommonStates::CullCounterClockwise,
            rtState);

        m_pEffect = std::make_unique<BasicEffect>(pDevice,
            EffectFlags::Lighting | EffectFlags::Texture, pd);
        m_pEffect->EnableDefaultLighting();
        m_pEffect->SetTexture(m_pResourceDescriptors->GetGpuHandle(Descriptors::Earth), m_pStates->AnisotropicWrap());
    }

    ResourceUploadBatch resourceUpload(pDevice);

    resourceUpload.Begin();
    if(!m_FolderLocation.empty())
    {
        const std::wstring location{ context.contentRoot + m_FolderLocation };

        const HRESULT result{ CreateWICTextureFromFile(pDevice,resourceUpload,location.c_str(),m_pTexture.ReleaseAndGetAddressOf(),false) };

        if (FAILED(result))
        {
            Logger::LogError(L"Cannot load texture in directory: " + location);
        }

        CreateShaderResourceView(pDevice, m_pTexture.Get(), m_pResourceDescriptors->GetCpuHandle(Descriptors::Earth));
    }

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
    m_pResourceDescriptors->Heap(), m_pStates->Heap()
    };
    commandList->SetDescriptorHeaps(std::size(heaps),
        heaps);

    const auto world{ m_pTransform->GetWorldMatrix() };
    m_pEffect->SetMatrices(world, view, proj);
    m_pEffect->Apply(commandList);
    m_pShape->Draw(commandList);
}

void MeshRenderComponent::OnDeviceLost()
{
    m_pShape.reset();
    m_pTexture.Reset();
    m_pResourceDescriptors.reset();
    m_pStates.reset();
    m_pEffect.reset();
}

void MeshRenderComponent::SetShape(std::unique_ptr<GeometricPrimitive>& shape)
{
    m_pShape = std::move(shape);
}