#include "pch.h"
#include "TestScene.h"

#include "Components/MeshRenderComponent.h"
#include "Managers/InputManager.h"

using namespace Engine;

void TestScene::Initialize()
{
	GameObject* room{ AddChild(new GameObject{}) };
	room->AddComponent<MeshRenderComponent>();


}

void TestScene::Update(const SceneContext&)
{
	const auto inputManager{ InputManager::GetInstance() };
	if(inputManager->IsReleased(L"test"))
	{
		Logger::LogInfo(L"test is a valid input");
	}
}
