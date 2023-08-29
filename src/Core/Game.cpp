//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

#include "Logger.h"
#include "Renderer.h"
#include "Components/TestComponent.h"
#include "SceneClasses/GameObject.h"
#include "Managers/SceneManager.h"
#include "SceneUtils.h"
#include "Command/Command.h"
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
    const auto pLogger{ Logger::GetInstance()
    };
    m_pRenderer->Initialize(window, width, height);
    const auto pGameObject{ std::make_unique<Engine::GameObject>() };
   if( !pGameObject->AddComponent<Engine::TestComponent>())
   {
       pLogger->LogError(L"Cannot add TestComponent!\n");
   }

   SceneUtils::LoadScenes();
   const auto input{ Engine::InputManager::GetInstance() };
   auto con{ XboxControllerButton::LSTICKDOWN };
   auto con2{ XboxControllerButton::B };
   auto con3{ XboxControllerButton::X };
   auto con4{ XboxControllerButton::Y };
   auto state{ DirectX::GamePad::ButtonStateTracker::RELEASED };
   input->BindButtonsToCommand(0,con,state,new Engine::TestCommand{} );
   input->BindButtonsToCommand(0,con2,state,new Engine::TestCommand{} );
   input->BindButtonsToCommand(0,con3,state,new Engine::TestCommand{} );
   input->BindButtonsToCommand(1,con4,state,new Engine::TestCommand{} );
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

    //const auto pad{ m_pGamePad->GetState(0) };
    //if(pad.IsConnected())
    //{
    //    m_buttons.Update(pad);
    //    if(pad.IsViewPressed())
    //    {
    //        ExitGame();
    //    }
    //}
    //else
    //{
    //    m_buttons.Reset();
    //}

    //if(m_buttons.a == GamePad::ButtonStateTracker::PRESSED)
    //{
	   // 
    //}
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
    //m_buttons.Reset();
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
    //if(m_pGamePad)
    //{
    //    m_pGamePad->Suspend();
    //}
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
    //if(m_pGamePad)
    //{
    //    m_pGamePad->Resume();
    //}
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
    //m_buttons.Reset();
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion
