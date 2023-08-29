#include "pch.h"
#include "Renderer.h"

#include "Logger.h"
#include "Managers/SceneManager.h"

namespace
{
    const DirectX::XMVECTORF32 START_POSITION = { 0.f, -1.5f, 0.f, 0.f };
    const DirectX::XMVECTORF32 ROOM_BOUNDS = { 8.f, 6.f, 12.f, 0.f };
    constexpr float ROTATION_GAIN = 0.1f;
}


Renderer::Renderer() : m_pitch{},m_yaw{},m_cameraPos{START_POSITION},m_roomColor{DirectX::Colors::White},
m_deviceResources{ std::make_unique<DX::DeviceResources>() }
{
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    m_deviceResources->RegisterDeviceNotify(this);
}

Renderer::~Renderer()
{
    if (m_deviceResources)
    {
        m_deviceResources->WaitForGpu();
    }
}

// Allocate all memory resources that change on a window SizeChanged event.
void Renderer::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.

    const auto size{ m_deviceResources->GetOutputSize() };
    m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(70.f),
        static_cast<float>(size.right) / static_cast<float>(size.bottom), 0.01f, 100.f);
}

void Renderer::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    // If using the DirectX Tool Kit for DX12, uncomment this line:
	m_pGraphicsMemory.reset();

    m_room.reset();
    m_roomTex.Reset();
    m_resourceDescriptors.reset();
    m_states.reset();
    m_roomEffect.reset();
}

void Renderer::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}

// These are the resources that depend on the device.
void Renderer::CreateDeviceDependentResources()
{
    auto device{ m_deviceResources->GetD3DDevice() };

    // Check Shader Model 6 support
    D3D12_FEATURE_DATA_SHADER_MODEL shaderModel = { D3D_SHADER_MODEL_6_0 };
    if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &shaderModel, sizeof(shaderModel)))
        || (shaderModel.HighestShaderModel < D3D_SHADER_MODEL_6_0))
    {
        const auto pLogger{ Logger::GetInstance() };
        pLogger->LogError(L"Shader Model 6.0 is not supported!");
    }

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_pGraphicsMemory = std::make_unique<DirectX::GraphicsMemory>(device);

    // TODO: Initialize device dependent objects here (independent of window size).
    m_resourceDescriptors = std::make_unique<DirectX::DescriptorHeap>(device, 1);
    m_states = std::make_unique<DirectX::CommonStates>(device);
    m_room = DirectX::GeometricPrimitive::CreateBox(DirectX::XMFLOAT3(ROOM_BOUNDS[0], ROOM_BOUNDS[1], ROOM_BOUNDS[2]),false,true);
    DirectX::RenderTargetState rtState{m_deviceResources->GetBackBufferFormat(), m_deviceResources->GetDepthBufferFormat()};
    {
        DirectX::EffectPipelineStateDescription pd(
            &DirectX::GeometricPrimitive::VertexType::InputLayout,
            DirectX::CommonStates::Opaque,
            DirectX::CommonStates::DepthDefault,
            DirectX::CommonStates::CullCounterClockwise,
            rtState);

        m_roomEffect = std::make_unique<DirectX::BasicEffect>(device, DirectX::EffectFlags::Lighting | DirectX::EffectFlags::Texture, pd);
        m_roomEffect->EnableDefaultLighting();
    }

    DirectX::ResourceUploadBatch resourceUpload(device);

    resourceUpload.Begin();

    DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, resourceUpload,
                                                        L"roomtexture.dds",
                                                        m_roomTex.ReleaseAndGetAddressOf()));

    DirectX::CreateShaderResourceView(device, m_roomTex.Get(),
                                      m_resourceDescriptors->GetFirstCpuHandle());

    m_roomEffect->SetTexture(m_resourceDescriptors->GetFirstGpuHandle(),
        m_states->LinearClamp());

    auto uploadResourcesFinished = resourceUpload.End(
        m_deviceResources->GetCommandQueue());
    uploadResourcesFinished.wait();

    m_deviceResources->WaitForGpu();

}

void Renderer::OnWindowMoved() const
{
    auto const r{ m_deviceResources->GetOutputSize() };
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Renderer::OnDisplayChange() const
{
    m_deviceResources->UpdateColorSpace();
}

void Renderer::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

void Renderer::Render() const
{
    const auto pSceneManager{ Engine::SceneManager::GetInstance() };
    // Prepare the command list to render a new frame.
    m_deviceResources->Prepare();
    Clear();

    const auto commandList{ m_deviceResources->GetCommandList() };
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");

    // TODO: Add your rendering code here.
    ID3D12DescriptorHeap* heaps[] = {
    m_resourceDescriptors->Heap(), m_states->Heap()
    };
    commandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)),
        heaps);

    m_roomEffect->SetMatrices(DirectX::SimpleMath::Matrix::Identity, m_view, m_proj);
    m_roomEffect->SetDiffuseColor(m_roomColor);
    m_roomEffect->Apply(commandList);
    m_room->Draw(commandList);

    pSceneManager->Render();

    PIXEndEvent(commandList);

    // Show the new frame.
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Present");
    m_deviceResources->Present();

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_pGraphicsMemory->Commit(m_deviceResources->GetCommandQueue());

    PIXEndEvent();
}

void Renderer::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
}

void Renderer::Update(float /*elapsedTime*/)
{
    constexpr float limit{ DirectX::XM_PIDIV2 - 0.01f };
    m_pitch = std::max(-limit, m_pitch);
    m_pitch = std::min(+limit, m_pitch);

    if(m_yaw > DirectX::XM_PI)
    {
        m_yaw -= DirectX::XM_2PI;
    }
    else if(m_yaw < -DirectX::XM_PI)
    {
        m_yaw += DirectX::XM_2PI;
    }

    const float y { sinf(m_pitch)};
    const float r { cosf(m_pitch)};
    const float z { r * cosf(m_yaw)};
    const float x{ r * sinf(m_yaw) };

   const DirectX::XMVECTOR lookAt = m_cameraPos + DirectX::SimpleMath::Vector3(x, y, z);

    m_view = XMMatrixLookAtRH(m_cameraPos, lookAt, DirectX::SimpleMath::Vector3::Up);
}

// Helper method to clear the back buffers.
void Renderer::Clear() const
{
    const auto commandList{ m_deviceResources->GetCommandList() };
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Clear");

    // Clear the views.
    auto const rtvDescriptor{ m_deviceResources->GetRenderTargetView() };
    auto const dsvDescriptor{ m_deviceResources->GetDepthStencilView() };

    commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
    commandList->ClearRenderTargetView(rtvDescriptor, DirectX::Colors::CornflowerBlue, 0, nullptr);
    commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // Set the viewport and scissor rect.
    auto const viewport{ m_deviceResources->GetScreenViewport() };
    auto const scissorRect{ m_deviceResources->GetScissorRect() };
    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissorRect);

    PIXEndEvent(commandList);
}