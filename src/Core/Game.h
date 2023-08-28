//
// Game.h
//

#pragma once

#include "../Utils/StepTimer.h"


// A basic game implementation that creates a D3D12 device and
// provides a game loop.

class Renderer;

namespace Engine
{
    class SceneManager;
}
class Game final
{
public:

    Game() noexcept(false);

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;
    Renderer* GetRenderer() const { return m_pRenderer.get(); }

private:

    void Update(DX::StepTimer const& timer);
    void Render() const;

    // Rendering loop timer.
    DX::StepTimer                               m_timer;

    std::unique_ptr<DirectX::GamePad> m_pGamePad{};
    Microsoft::WRL::ComPtr<ID3D12Device> m_d3dDevice;

    Engine::SceneManager* m_pSceneManager{};
    std::unique_ptr<Renderer> m_pRenderer{};
};
