//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

#include "Renderer.h"
#include "Managers/SceneManager.h"
#include "SceneUtils.h"
#include "Managers/InputManager.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false) : m_pSceneManager{ Engine::SceneManager::GetInstance() }, m_pRenderer{std::make_unique<Renderer>()}
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_pRenderer->Initialize(window, width, height);
   SceneUtils::LoadScenes();
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    const float elapsedTime{ static_cast<float>(timer.GetElapsedSeconds()) };

    // TODO: Add your game logic here.
    Engine::InputManager::GetInstance()->ProcessControllerInput(elapsedTime);
    Engine::InputManager::GetInstance()->Update();
    m_pSceneManager->Update(elapsedTime);

    PIXEndEvent();

}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render() const
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }
    ; m_pRenderer->Render();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
    const auto inputManager{ Engine::InputManager::GetInstance() };
    inputManager->Resume();
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window
    // causes leak
	const auto inputManager{ Engine::InputManager::GetInstance() };
    inputManager->Suspend();
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
    const auto inputManager{ Engine::InputManager::GetInstance() };
    inputManager->Suspend();
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
    const auto inputManager{ Engine::InputManager::GetInstance() };
    inputManager->Resume();
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion
