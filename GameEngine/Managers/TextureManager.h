#pragma once
#include "pch.h"
#include "Main/Singleton.h"
#include <unordered_map>

namespace Engine
{
	class TextureManager : public Singleton<TextureManager>
	{
	public:
		[[nodiscard]] Microsoft::WRL::ComPtr<ID3D12Resource> GetTexture(const std::wstring& textureName);

		void OnDeviceLost();

		void Clean()
		{
			m_Textures.clear();
		}
	private:

		Microsoft::WRL::ComPtr<ID3D12Resource> LoadTexture(const std::wstring& textureName);
		std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3D12Resource>> m_Textures;
	};
}
