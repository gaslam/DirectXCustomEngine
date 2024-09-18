//
// Game.h
//

#pragma once

#include "Main/StepTimer.h"


// A basic game implementation that creates a D3D12 device and
// provides a game loop.

class Renderer;

namespace EntityEngine
{
    class Command;
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
    static void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnDeviceLost();
    void OnWindowSizeChanged(const int width, const int height);
    void OnResuming();

    // Properties
    void GetDefaultSize(int& width, int& height) const noexcept;
    [[nodiscard]] Renderer* GetRenderer() const { return m_pRenderer.get(); }

private:

    void Update();
    void Render() const;

    // Rendering loop timer.
    DX::StepTimer* m_pTimer;

    Microsoft::WRL::ComPtr<ID3D12Device> m_d3dDevice;

    EntityEngine::SceneManager* m_pSceneManager{};
    std::unique_ptr<Renderer> m_pRenderer{};
};
