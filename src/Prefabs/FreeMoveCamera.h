#pragma once
#include "SceneClasses/GameObject.h"

namespace Engine
{
	class InputManager;
    class CameraComponent;
}

using namespace Engine;

class FreeMoveCamera :
    public GameObject
{
public:

    void Initialize(const SceneContext&) override;
    void SetMovementSpeed(const float speed) { m_MovementSpeed = speed; }

protected:
    void Update(const SceneContext& context) override;
private:
    float m_MovementSpeed{10.f};
    float m_RotationSpeed{10.f};

    CameraComponent* m_pComponent{ nullptr };

    const std::wstring CAMERA_FORWARD_CONTROL{ L"forward" };
    const std::wstring CAMERA_BACKWARD_CONTROL{ L"backward" };
    const std::wstring CAMERA_LEFT_CONTROL{ L"left" };
    const std::wstring CAMERA_RIGHT_CONTROL{ L"right" };

    void UpdateCameraMovement(const SceneContext& context);
    void RotateCamera(InputManager* input, float elapsedTime) const;
    void TranslateCamera(InputManager* input, float elapsedTime) const;
};

