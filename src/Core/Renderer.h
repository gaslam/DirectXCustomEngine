#pragma once
#include "DeviceResources.h"
class Renderer : public DX::IDeviceNotify
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

	//to be removed
	void Update(float elapsedTime);
private:
	void Clear() const;
	// Device resources.
	std::unique_ptr<DX::DeviceResources>        m_deviceResources{};
	// If using the DirectX Tool Kit for DX12, uncomment this line:
	std::unique_ptr<DirectX::GraphicsMemory> m_pGraphicsMemory{};

	std::unique_ptr<DirectX::GeometricPrimitive> m_room;

	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	float m_pitch;
	float m_yaw;
	DirectX::SimpleMath::Vector3 m_cameraPos;

	DirectX::SimpleMath::Color m_roomColor;

	Microsoft::WRL::ComPtr<ID3D12Resource> m_roomTex;
std::unique_ptr<DirectX::DescriptorHeap> m_resourceDescriptors;
std::unique_ptr<DirectX::CommonStates> m_states;
std::unique_ptr<DirectX::BasicEffect> m_roomEffect;
};

