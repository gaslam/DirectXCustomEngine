#pragma once
#include "pch.h"
#include "Managers/SceneManager.h"
#include "SceneClasses/Scene.h"

using namespace DirectX;
using namespace SimpleMath;

namespace EntityEngine {
	class TransformComponentECS;
	class InputManager;
	class CameraComponentECS final {
		float m_Pitch{};
		float m_Roll{};
		float m_Yaw{};
		float m_FOV{ };
		float m_RotationSpeed{ 1.f };
		float m_MovementSpeed{ 1.f };
		bool m_MarkedForScreenUpdate{ true };

		    const std::wstring CAMERA_FORWARD_CONTROL{ L"forward" };
    const std::wstring CAMERA_BACKWARD_CONTROL{ L"backward" };
    const std::wstring CAMERA_LEFT_CONTROL{ L"left" };
    const std::wstring CAMERA_RIGHT_CONTROL{ L"right" };

		TransformComponentECS* m_pTransform{};

		Vector3 m_CurrentPos{};

		Matrix m_View{};
		Matrix m_Proj{};
		Matrix m_ViewProj{};

		void ChangeProjection();
		void UpdateCameraMovement();

		void RotateCamera(InputManager* input, float elapsedTime);

		void TranslateCamera(InputManager* input, float elapsedTime);

		void MoveCamera(const Vector3& pos);

	public:
		CameraComponentECS() {};

		void Initialize(TransformComponentECS* currentPos, float pitch = 0.f, float roll = 0.f, float yaw = 0.f, float fov = 70.f);
		void Update();
	};
}