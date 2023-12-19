#pragma once
#include <Core/DeviceResources.h>

#include "Utils/StepTimer.h"

namespace Engine
{
	class CameraComponent;
}

struct DX12Context
{
	DX::DeviceResources* pDeviceResources;
};

struct GameContext
{
	int windowWidth{}, windowHeight{};
	std::wstring windowTitle{ L"Custom title!!" };
	std::wstring contentRoot{ L"./Resources/" };
	DX12Context d12Context{};
	DX::StepTimer* timer{};

};

struct SceneContext {
	Engine::CameraComponent* pCameraComp{};
	DX12Context d12Context{};
	int windowWidth{}, windowHeight{};
	std::wstring contentRoot{ L"./Resources/" };
	DX::StepTimer* timer;
};