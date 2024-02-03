#pragma once
#include "Core/DeviceResources.h"
#include "Main/Logger.h"


class GameHandlerBase
{
public:
	GameHandlerBase() = default;
	virtual ~GameHandlerBase() = default;

	virtual void Initialize() = 0;

	virtual [[nodiscard]] std::wstring GetContentRoot() const = 0;
	virtual [[nodiscard]] DX::DeviceResources* GetDeviceResources() const = 0;
	virtual [[nodiscard]] ID3D12Device* GetDevice() const = 0;
	virtual [[nodiscard]] std::wstring GetWindowTitle() const = 0;

	virtual void SetDeviceResources(DX::DeviceResources*) = 0;
	virtual void SetDevice(ID3D12Device*) = 0;
	virtual void GetWindowWidthAndHeight(int& width, int& height) const = 0;
	virtual void SetWindowWidthAndHeight(int width, int height) = 0;

	virtual void SetWindowTitle(std::wstring&) = 0;
};


class NullGameHandler : public GameHandlerBase
{
public:

	void Initialize() override {};

	[[nodiscard]] std::wstring GetContentRoot() const override { return L""; }
	virtual [[nodiscard]] std::wstring GetWindowTitle() const override { return L""; }
	[[nodiscard]] DX::DeviceResources* GetDeviceResources() const override { return nullptr; }
	[[nodiscard]] ID3D12Device* GetDevice() const override { return nullptr; }

	void SetDeviceResources(DX::DeviceResources*)  override {};
	void SetDevice(ID3D12Device*) override {};
	void GetWindowWidthAndHeight(int&, int&) const override {};
	void SetWindowWidthAndHeight(int, int) override {};

	void SetWindowTitle(std::wstring&) override{};
};
class GameHandler : public  GameHandlerBase
{
public:

	[[nodiscard]] std::wstring GetContentRoot() const override { return m_ContentRoot; }
	[[nodiscard]] DX::DeviceResources* GetDeviceResources() const override { return m_pDeviceResources; }
	[[nodiscard]] ID3D12Device* GetDevice() const override { return m_pDevice; }
	[[nodiscard]] std::wstring GetWindowTitle() const override { return m_WindowTitle; }

	void Initialize() override
	{
		DisplayWindowTitle();
	}

	void SetWindowTitle(std::wstring&title) override
	{
		m_WindowTitle = title;
		DisplayWindowTitle();
	}

	void SetDeviceResources(DX::DeviceResources* pResources) override
	{
		if (pResources == nullptr)
		{
			Logger::LogError(L"Cannot set device resources to null!! Make sure it is always available");
			return;
		}
		m_pDeviceResources = pResources;
	}

	void SetDevice(ID3D12Device* pDevice) override
	{
		if (pDevice == nullptr)
		{
			Logger::LogError(L"Cannot set device to null!! Make sure it is always available");
			return;
		}
		m_pDevice = pDevice;
	}

	void GetWindowWidthAndHeight(int& width, int& height) const override
	{
		width = m_WindowWidth;
		height = m_WindowHeight;
	}

	void SetWindowWidthAndHeight(int width, int height) override
	{
		m_WindowWidth = width;
		m_WindowHeight = height;
	}
private:

	void DisplayWindowTitle() const
	{
		const HWND window{ GetActiveWindow() };
		SetWindowTextW(window, m_WindowTitle.c_str());
	}

	int m_WindowWidth{}, m_WindowHeight{};
	std::wstring m_ContentRoot{ L"./Resources/" };
	DX::DeviceResources* m_pDeviceResources{};
	ID3D12Device* m_pDevice{};
	std::wstring m_WindowTitle{ L"DirectXCustomEngine" };

};