#pragma once
#include "BaseComponent.h"
#include "pch.h"

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
        void RotateCamera(Vector3 rot,bool isDegrees = false);

        CameraComponent(const CameraComponent& other) = delete;
        CameraComponent(CameraComponent&& other)noexcept = delete;
        CameraComponent& operator=(const CameraComponent& other) = delete;
        CameraComponent& operator=(CameraComponent&& other)noexcept = delete;

        void ChangeProjection();
        void Initialize(Scene* pScene) override;
        void MoveCamera(Vector3 pos);
        [[nodiscard]]Matrix GetProjectionMatrix() const { return m_Proj; }
        [[nodiscard]]Matrix GetViewMatrix() const { return m_View; }
        [[nodiscard]]TransformComponent* GetTransform() const { return m_Transform; }
        void SetFOV(float fov);
    private:

        float m_Pitch{};
        float m_Roll{};
        float m_Yaw{};
        float m_FOV{ 70.f };
        bool m_MarkedForScreenUpdate{ true };

        Vector3 m_CurrentPos{};
        
        Matrix m_View{};
        Matrix m_ViewInverse{};
        Matrix m_Proj{};
        Matrix m_ProjInverse{};
        Matrix m_ViewProj{};
        Matrix m_ViewProjInverse{};

        TransformComponent* m_Transform{};

        void OnScreensizeChanged(const int, const int) override;
    };
}

