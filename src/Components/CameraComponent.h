#pragma once
#include "BaseComponent.h"

using namespace DirectX;
using namespace SimpleMath;

class TransformComponent;
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
        void ChangeProjection(const SceneContext& context);
        void Initialize(const SceneContext& context) override;
        Matrix GetProjectionMatrix() const { return m_Proj; }
        Matrix GetViewMatrix() const { return m_View; }
        void SetFOV(float fov);
    private:
        Matrix m_View{};
        Matrix m_ViewInverse{};
        Matrix m_Proj{};
        Matrix m_ProjInverse{};
        Matrix m_ViewProj{};
        Matrix m_ViewProjInverse{};

        float m_Pitch{};
        float m_Roll{};
        float m_Yaw{};
        float m_FOV{ 70.f };
        bool m_MarkedForScreenUpdate{true};
        TransformComponent* m_Transform;

        void OnScreensizeChanged(const int, const int) override;
    };
}

