#pragma once
#include "SceneClasses/GameObject.h"

namespace EntityEngine
{
	class InputManager;
    class CameraComponent;
}

using namespace EntityEngine;

class FreeMoveCamera :
    public GameObject
{
public:

	FreeMoveCamera() = default;

    //TODO: Fix warnings that are caused by the default constructor being deleted in base class. For now this works

	~FreeMoveCamera() override = default;

    FreeMoveCamera(const FreeMoveCamera& other) = delete;
    FreeMoveCamera(FreeMoveCamera&& other) noexcept = delete;
    FreeMoveCamera& operator=(const FreeMoveCamera& other) = delete;
    FreeMoveCamera& operator=(FreeMoveCamera&& other) noexcept = delete;

    void Initialize(Scene*) override;
    void SetMovementSpeed(const float speed) { m_MovementSpeed = speed; }

protected:
    void RenderImGui() override;
private:
    float m_MovementSpeed{10.f};
    float m_RotationSpeed{10.f};

    CameraComponent* m_pComponent{ nullptr };

    const std::wstring CAMERA_FORWARD_CONTROL{ L"forward" };
    const std::wstring CAMERA_BACKWARD_CONTROL{ L"backward" };
    const std::wstring CAMERA_LEFT_CONTROL{ L"left" };
    const std::wstring CAMERA_RIGHT_CONTROL{ L"right" };

    void UpdateCameraMovement();
    void RotateCamera(InputManager* input, float elapsedTime) const;
    void TranslateCamera(InputManager* input, float elapsedTime) const;
};

