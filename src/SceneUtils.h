#pragma once
#include <memory>
#include "Managers/SceneManager.h"
#include "SceneClasses/TestScene.h"
// I am planning to replace this with a file reader

namespace SceneUtils
{
	inline void LoadScenes()
	{
		Engine::SceneManager* pSceneManager{Engine::SceneManager::GetInstance()};
		pSceneManager->AddScene(std::make_unique<Engine::TestScene>());
		pSceneManager->SetScene(0);
	}
}
