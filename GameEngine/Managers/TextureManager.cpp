#include "TextureManager.h"

#include <ranges>

#include "Utils/GameHandler.h"
#include <string>

namespace Engine
{
	ID3D12Resource* TextureManager::GetTexture(const std::wstring& textureName)
	{
		auto texturePair{ m_Textures.find(textureName)};
		if (texturePair == m_Textures.end())
		{
			return LoadTexture(textureName);
		}
		return texturePair->second.Get();
	}
	void TextureManager::OnDeviceLost()
	{
		for (auto& val : m_Textures | views::values)
		{
			val.Reset();
		}
	}
	ID3D12Resource* TextureManager::LoadTexture(const std::wstring& textureName)
	{
		auto pHandler{ Locator::GetGameHandler() };
		const std::wstring textureLocation{ pHandler->GetContentRoot() + L"Textures/" + textureName  };
		auto pDevice{ pHandler->GetDevice() };
		auto pDeviceResources{ pHandler->GetDeviceResources() };
		auto resourceUpload{ DirectX::ResourceUploadBatch(pDevice)};

		resourceUpload.Begin();

		ID3D12Resource* pTexture{ nullptr };

		const HRESULT result{DirectX::CreateWICTextureFromFile(pDevice,resourceUpload,textureLocation.c_str(),&pTexture,false) };

		if (FAILED(result))
		{
			Logger::LogError(L"Cannot load texture in directory: " + textureLocation);
		}

		const future<void> uploadResourcesFinished{ resourceUpload.End(
	pDeviceResources->GetCommandQueue()) };
		uploadResourcesFinished.wait();

		pDeviceResources->WaitForGpu();

		m_Textures[textureName] = pTexture;
		return pTexture;
	}
}
