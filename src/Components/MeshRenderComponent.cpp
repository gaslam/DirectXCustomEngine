#include "pch.h"
#include "MeshRenderComponent.h"

#include "CameraComponent.h"

using namespace DirectX;

void MeshRenderComponent::Initialize(const SceneContext& context)
{
    DX::DeviceResources* pDeviceResources{ context.d12Context.pDeviceResources };
    ID3D12Device* pDevice{ pDeviceResources->GetD3DDevice() };
    m_resourceDescriptors = std::make_unique<DescriptorHeap>(pDevice, 1);

    m_states = std::make_unique<CommonStates>(pDevice);

    m_room = GeometricPrimitive::CreateBox(
        XMFLOAT3(ROOM_BOUNDS[0], ROOM_BOUNDS[1], ROOM_BOUNDS[2]),
        false, true);

    RenderTargetState rtState(pDeviceResources->GetBackBufferFormat(),
        pDeviceResources->GetDepthBufferFormat());

    {
        EffectPipelineStateDescription pd(
            &GeometricPrimitive::VertexType::InputLayout,
            CommonStates::Opaque,
            CommonStates::DepthDefault,
            CommonStates::CullCounterClockwise,
            rtState);

        m_roomEffect = std::make_unique<BasicEffect>(pDevice,
            EffectFlags::Lighting | EffectFlags::Texture, pd);
        m_roomEffect->EnableDefaultLighting();
    }

    ResourceUploadBatch resourceUpload(pDevice);

    resourceUpload.Begin();

    const std::wstring fileLocation{ L"./Resources/roomtexture.dds" };
    const HRESULT result = CreateDDSTextureFromFile(pDevice, resourceUpload, fileLocation.c_str()
        ,
        m_roomTex.ReleaseAndGetAddressOf());

    if(FAILED(result))
    {
        std::wstring error{ L"Cannot create texture from file location: " + fileLocation };
        Logger::LogError(error);
        return;
    }

    CreateShaderResourceView(pDevice, m_roomTex.Get(),
        m_resourceDescriptors->GetFirstCpuHandle());

    m_roomEffect->SetTexture(m_resourceDescriptors->GetFirstGpuHandle(),
        m_states->LinearClamp());

    const future<void> uploadResourcesFinished{ resourceUpload.End(
        pDeviceResources->GetCommandQueue()) };
    uploadResourcesFinished.wait();

    pDeviceResources->WaitForGpu();

    m_RoomColor = Colors::White;
}

void MeshRenderComponent::Render(const SceneContext& context)
{
    const auto pCamera{ context.pCameraComp };
    const Matrix view{ pCamera->GetViewMatrix() };
    const Matrix proj{ pCamera->GetProjectionMatrix() };
    const auto commandList{ context.d12Context.pDeviceResources->GetCommandList() };
    ID3D12DescriptorHeap* heaps[] = {
    m_resourceDescriptors->Heap(), m_states->Heap()
    };
    commandList->SetDescriptorHeaps(std::size(heaps),
        heaps);

    m_roomEffect->SetMatrices(Matrix::Identity, view, proj);
    m_roomEffect->SetDiffuseColor(m_RoomColor);
    m_roomEffect->Apply(commandList);
    m_room->Draw(commandList);
}

void MeshRenderComponent::OnDeviceLost()
{
    m_room.reset();
    m_roomTex.Reset();
    m_resourceDescriptors.reset();
    m_states.reset();
    m_roomEffect.reset();
}
