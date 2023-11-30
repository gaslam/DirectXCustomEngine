#include "pch.h"
#include "Renderer.h"

#include "Logger.h"
#include "Managers/SceneManager.h"


Renderer::Renderer() :
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

}

void Renderer::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    // If using the DirectX Tool Kit for DX12, uncomment this line:
	m_pGraphicsMemory.reset();
    const auto pSceneManager{ Engine::SceneManager::GetInstance() };
    pSceneManager->OnDeviceLost();
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