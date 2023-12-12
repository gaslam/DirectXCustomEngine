#include "pch.h"
#include "FreeMoveCamera.h"
#include "Components/CameraComponent.h"
#include "Managers/InputManager.h"

void FreeMoveCamera::Initialize(const SceneContext&)
{
	m_pComponent = AddComponent<CameraComponent>();
	const auto input{ InputManager::GetInstance() };
	auto leftKey { DirectX::Keyboard::Keys::A };
	auto rightKey { DirectX::Keyboard::Keys::D };
	auto backKey { DirectX::Keyboard::Keys::S };
	auto forwardKey { DirectX::Keyboard::Keys::W };
	input->BindButtonsToInput(0, CAMERA_LEFT_CONTROL,leftKey);
	input->BindButtonsToInput(0, CAMERA_FORWARD_CONTROL,forwardKey);
	input->BindButtonsToInput(0, CAMERA_BACKWARD_CONTROL,backKey);
	input->BindButtonsToInput(0, CAMERA_RIGHT_CONTROL,rightKey);
}

void FreeMoveCamera::Update(const SceneContext& context)
{
	UpdateCameraMovement(context);
}

void FreeMoveCamera::UpdateCameraMovement(const SceneContext& context)
{
	const float elapsedTime{ static_cast<float>(context.timer->GetElapsedSeconds()) };
	const auto input{ InputManager::GetInstance() };
	TranslateCamera(input, elapsedTime);
	RotateCamera(input, elapsedTime);
}

void FreeMoveCamera::RotateCamera(InputManager* input, float elapsedTime) const
{

	Vector2 moveDir{ input->GetMouseDelta() };

	if (moveDir.LengthSquared() == 0.f)
	{
		moveDir = input->GetJoystickCoords(XboxJoystick::LEFT, 0);
	}

	Vector3 rot{ moveDir.x,moveDir.y,0.f };
	rot *= elapsedTime * m_RotationSpeed;
	m_pComponent->RotateCamera(rot, true);
}
void FreeMoveCamera::TranslateCamera(InputManager* input, float elapsedTime) const
{
	const auto transform{ m_pComponent->GetTransform() };

	bool isPressed{ false };
	Vector2 dir{};
	if (input->IsHeld(CAMERA_FORWARD_CONTROL))
	{
		dir.y += 1;
		isPressed = true;
	}

	if (input->IsHeld(CAMERA_BACKWARD_CONTROL))
	{
		dir.y -= 1;
		isPressed = true;
	}

	if (input->IsHeld(CAMERA_LEFT_CONTROL))
	{
		dir.x -= 1;
		isPressed = true;
	}

	if (input->IsHeld(CAMERA_RIGHT_CONTROL))
	{
		dir.x += 1;
		isPressed = true;
	}

	if (!isPressed)
	{
		dir = input->GetJoystickCoords(XboxJoystick::LEFT, 0);
		dir.Normalize();
	}
	Vector3 dirToGo{};
	dirToGo += transform->GetForward() * dir.y * m_MovementSpeed * elapsedTime;
	dirToGo += dir.x * transform->GetRight() * m_MovementSpeed * elapsedTime;

	m_pComponent->MoveCamera(dirToGo);
}

