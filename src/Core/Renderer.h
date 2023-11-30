#pragma once
#include "DeviceResources.h"
class Renderer final : public DX::IDeviceNotify
{
public:
	Renderer();
	~Renderer();

	Renderer(const Renderer& other) = delete;
	Renderer(Renderer&& other) noexcept = delete;
	Renderer& operator=(const Renderer& other) = delete;
	Renderer& operator=(Renderer&& other) noexcept = delete;

	void CreateWindowSizeDependentResources();
	void CreateDeviceDependentResources();
	// IDeviceNotify
	void OnDeviceLost() override;
	void OnDeviceRestored() override;
	void OnWindowMoved() const;
	void OnDisplayChange() const;
	void OnWindowSizeChanged(int width, int height);
	void Render() const;
	void Initialize(HWND window, int width, int height);
	DX::DeviceResources* GetDeviceResources() const { return  m_deviceResources.get(); }

	//to be removed
	void Update(float elapsedTime);
private:
	void Clear() const;
	// Device resources.
	std::unique_ptr<DX::DeviceResources>        m_deviceResources{};
	// If using the DirectX Tool Kit for DX12, uncomment this line:
	std::unique_ptr<DirectX::GraphicsMemory> m_pGraphicsMemory{};
};

