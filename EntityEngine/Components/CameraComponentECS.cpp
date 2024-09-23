#include "pch.h"
#include "CameraComponentECS.h"
#include "TransformComponentECS.h"
#include "Managers/InputManager.h"

namespace EntityEngine {
	void CameraComponentECS::Initialize(TransformComponentECS* currentPos, float pitch, float roll, float yaw, float fov)
	{
		m_pTransform = currentPos;
		m_Pitch = pitch;
		m_Roll = roll;
		m_Yaw = yaw;
		m_FOV = fov;

		const auto input{ InputManager::GetInstance() };
		auto leftKey{ DirectX::Keyboard::Keys::A };
		auto rightKey{ DirectX::Keyboard::Keys::D };
		auto backKey{ DirectX::Keyboard::Keys::S };
		auto forwardKey{ DirectX::Keyboard::Keys::W };
		input->BindButtonsToInput(0, CAMERA_LEFT_CONTROL, leftKey);
		input->BindButtonsToInput(0, CAMERA_FORWARD_CONTROL, forwardKey);
		input->BindButtonsToInput(0, CAMERA_BACKWARD_CONTROL, backKey);
		input->BindButtonsToInput(0, CAMERA_RIGHT_CONTROL, rightKey);
	}

	void CameraComponentECS::ChangeProjection()
	{
		const auto pHandler{ Locator::GetGameHandler() };
		int width{}, height{};

		pHandler->GetWindowWidthAndHeight(width, height);

		m_Proj = Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(m_FOV),
			static_cast<float>(width) / static_cast<float>(height), 0.001f, 50.f);
	}
	void CameraComponentECS::Update()
	{
		if (m_MarkedForScreenUpdate)
		{
			ChangeProjection();
			m_MarkedForScreenUpdate = false;
		}

		if (!m_pTransform)
		{
			return;
		}

		UpdateCameraMovement();
		const Vector3 camPos{ m_pTransform->GetWorldPosition() };

		const Vector3 lookAt{ camPos + m_pTransform->GetForward() };
		m_View = XMMatrixLookAtRH(camPos, lookAt, Vector3::Up);
		m_ViewProj = m_View * m_Proj;
	}

	void CameraComponentECS::UpdateCameraMovement()
	{
		const auto timer{ Locator::GetTimer() };
		const float elapsedTime{ static_cast<float>(timer->GetElapsedSeconds()) };
		const auto input{ InputManager::GetInstance() };
		TranslateCamera(input, elapsedTime);
		RotateCamera(input, elapsedTime);
	}

	void CameraComponentECS::RotateCamera(InputManager* input, float elapsedTime)
	{

		Vector2 moveDir{ input->GetMouseDelta() };

		if (moveDir.LengthSquared() == 0.f)
		{
			moveDir = input->GetJoystickCoords(XboxJoystick::LEFT, 0);
		}

		if (moveDir.LengthSquared() == 0.f)
		{
			return;
		}

		Vector3 rot{ moveDir.x,moveDir.y,0.f };
		rot *= elapsedTime * m_RotationSpeed;
		m_Pitch += rot.x;
		m_Yaw += rot.z;
		m_Roll -= rot.y;

		m_pTransform->Rotate(m_Roll, m_Pitch, m_Yaw,false);
	}
	void CameraComponentECS::TranslateCamera(InputManager* input, float elapsedTime)
	{

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
		dirToGo += m_pTransform->GetForward() * dir.y * m_MovementSpeed * elapsedTime;
		dirToGo += dir.x * m_pTransform->GetRight() * m_MovementSpeed * elapsedTime;

		MoveCamera(dirToGo);
	}

	void CameraComponentECS::MoveCamera(const Vector3& pos)
{
	if(m_pTransform)
	{
		m_CurrentPos += pos;
		m_pTransform->Translate(m_CurrentPos);
	}
}
}