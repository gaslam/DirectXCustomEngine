#include "pch.h"
#include "TestScene.h"

#include "KeyStructs/KeyStructs.h"
#include "Managers/InputManager.h"

using namespace Engine;

void TestScene::Initialize()
{
	const auto input{ InputManager::GetInstance() };
	auto con{ XboxControllerButton::B };
	auto conK{ DirectX::Keyboard::Keys::A };
	input->BindButtonsToInput(0, con,conK,L"test");

}

void TestScene::Update(float)
{
	const auto inputManager{ InputManager::GetInstance() };
	if(inputManager->IsReleased(L"test"))
	{
		const auto logger{ Logger::GetInstance() };
		logger->LogInfo(L"test is a valid input");
	}
}
