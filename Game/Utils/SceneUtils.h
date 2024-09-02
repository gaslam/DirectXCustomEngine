#pragma once
#include "Managers/SceneManager.h"
#include "../Scene/TestScene.h"
// I am planning to replace this with a file reader

namespace SceneUtils
{
	inline void LoadScenes()
	{
		Engine::SceneManager* pSceneManager{Engine::SceneManager::GetInstance()};
		pSceneManager->AddScene(std::make_unique<Exe::TestScene>(L"TestScene"));
		pSceneManager->SetScene(0);
	}
}
