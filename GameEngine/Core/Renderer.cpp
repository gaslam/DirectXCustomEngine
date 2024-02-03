#include "pch.h"
#include "Renderer.h"

#ifdef _DEBUG
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#endif
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
#ifdef _DEBUG
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	m_pDescriptorHeaps->Release();
#endif
}

// Allocate all memory resources that change on a window SizeChanged event.
void Renderer::CreateWindowSizeDependentResources()
{
	// TODO: Initialize windows-size dependent objects here.

}

void Renderer::OnDeviceLost()
{
	// TODO: Add Direct3D resource cleanup here.
	m_deviceResources->WaitForGpu();
	m_pGraphicsMemory.reset();
	m_pDescriptorHeaps->Release();
	const auto pSceneManager{ Engine::SceneManager::GetInstance() };
	pSceneManager->OnDeviceLost();

	const auto pTimer{ Locator::GetTimer() };
	pTimer->ResetElapsedTime();

#ifdef _DEBUG
	ImGui::EndFrame();
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif
}

void Renderer::OnDeviceRestored()
{
	CreateDeviceDependentResources();

	CreateWindowSizeDependentResources();
	InitImGui();
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

	auto pHandler{ Locator::GetGameHandler() };
	pHandler->SetDevice(device);
	pHandler->SetDeviceResources(m_deviceResources.get());

	// If using the DirectX Tool Kit for DX12, uncomment this line:
	m_pGraphicsMemory = std::make_unique<DirectX::GraphicsMemory>(device);

	// TODO: Initialize device dependent objects here (independent of window size).
	Engine::SceneManager::GetInstance()->InitDeviceResources();

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

void Renderer::Render()
{
	// Prepare the command list to render a new frame.
	m_deviceResources->Prepare();
	Clear();
	auto commandList = m_deviceResources->GetCommandList();
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");

	// TODO: Add your rendering code here.
	SceneManager::GetInstance()->Render();
#ifdef _DEBUG
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// We specify a default position/size in case there's no data in the .ini file.
	// We only do it to make the demo applications a little more welcoming, but typically this isn't required.
	//bool isActive{ true };
	//ImGui::ShowDemoWindow(&isActive);
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 50, main_viewport->WorkPos.y + 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_Once);

	ImGuiWindowFlags windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_MenuBar;
	if (!ImGui::Begin("DirectX12 custom engine", NULL, windowFlags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
	const auto timer{ Locator::GetTimer() };
	const float fps{ static_cast<float>(timer->GetFramesPerSecond()) };
	ImGui::Text("MS: %.3f", 1000.0f/fps);
	ImGui::Text("FPS: %.0f", fps);

	bool vsyncPreviousState{ m_IsVSyncEnabled };
	ImGui::Text("VSync: ");
	ImGui::SameLine();
	ImGui::Checkbox("##VSyncCheckbox", &m_IsVSyncEnabled);
	if(vsyncPreviousState != m_IsVSyncEnabled)
	{
		timer->SetFixedTimeStep(m_IsVSyncEnabled);
	}
	SceneManager::GetInstance()->RenderImGui();
	ImGui::End();
	ImGui::Render();


	commandList->SetDescriptorHeaps(1, &m_pDescriptorHeaps);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault(nullptr, (void*)m_deviceResources->GetCommandQueue());
	}
#endif
	PIXEndEvent(commandList);
	// Show the new frame.
	PIXBeginEvent(m_deviceResources->GetCommandQueue(), PIX_COLOR_DEFAULT, L"Present");
	m_deviceResources->Present();
	PIXEndEvent(m_deviceResources->GetCommandQueue());
}

#ifdef _DEBUG
void Renderer::InitImGui()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	if (m_pDescriptorHeaps)
	{
		m_pDescriptorHeaps->Release();
	}
	if (m_deviceResources->GetD3DDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pDescriptorHeaps)) != S_OK)
	{
		Logger::LogError(L"Cannot render imgui!!");
		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	io.ConfigViewportsNoAutoMerge = true;
	io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(m_Window);
	ImGui_ImplDX12_Init(m_deviceResources->GetD3DDevice(), 3,
		DXGI_FORMAT_R8G8B8A8_UNORM, m_pDescriptorHeaps,
		m_pDescriptorHeaps->GetCPUDescriptorHandleForHeapStart(),
		m_pDescriptorHeaps->GetGPUDescriptorHandleForHeapStart());
#endif
}


void Renderer::Initialize(HWND window, int width, int height)
{
	m_Window = window;
	m_deviceResources->SetWindow(window, width, height);

	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

	InitImGui();

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
	auto commandList = m_deviceResources->GetCommandList();

	// Clear the views.
	auto const rtvDescriptor = m_deviceResources->GetRenderTargetView();
	auto const dsvDescriptor = m_deviceResources->GetDepthStencilView();

	commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
	commandList->ClearRenderTargetView(rtvDescriptor, Colors::CornflowerBlue, 0, nullptr);
	commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// Set the viewport and scissor rect.
	auto const viewport = m_deviceResources->GetScreenViewport();
	auto const scissorRect = m_deviceResources->GetScissorRect();
	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissorRect);
}