#pragma once
#include "BaseComponent.h"

using namespace DirectX;
using namespace SimpleMath;

namespace Engine
{
    class CameraComponent :
        public BaseComponent
    {
    public:
        CameraComponent() = default;
        ~CameraComponent() override = default;

        CameraComponent(const CameraComponent& other) = delete;
        CameraComponent(CameraComponent&& other)noexcept = delete;
        CameraComponent& operator=(const CameraComponent& other) = delete;
        CameraComponent& operator=(CameraComponent&& other)noexcept = delete;

        void Update(const SceneContext& context) override;
        void Initialize(const SceneContext& context) override;
        Matrix GetProjectionMatrix() const { return m_proj; }
        Matrix GetViewMatrix() const { return m_view; }
    private:
        Matrix m_view{};
        Matrix m_proj{};

        float m_pitch{};
        float m_yaw{};
        Vector3 m_cameraPos;
    };
}

