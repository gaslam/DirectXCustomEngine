#include "CameraComponent.h"

#include "SceneClasses/GameObject.h"
#include "Components/TransformComponent.h"
#include "SceneClasses/Scene.h"

using namespace EntityEngine;

void CameraComponent::RotateCamera(Vector3 rot, bool isDegrees)
{
	m_Pitch += rot.x;
	m_Yaw += rot.z;
	m_Roll -= rot.y;

	m_Transform->Rotate(m_Roll, m_Pitch, m_Yaw,isDegrees);
}

void CameraComponent::ChangeProjection()
{
	const auto pHandler{ Locator::GetGameHandler() };
	int width{}, height{};

	pHandler->GetWindowWidthAndHeight(width, height);

	m_Proj = Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(m_FOV),
		static_cast<float>(width) / static_cast<float>(height), 0.001f, 50.f);

}

void CameraComponent::Initialize(Scene* pScene)
{
	const GameObject* owner{ GetOwner() };

	if(owner == nullptr)
	{
		Logger::LogWarning(L"Cannot get transform on camera!!");
		return;
	}

	m_Transform = owner->GetTransform();

	pScene->AddUpdateCallback([&](const Scene*)
		{
			if (m_MarkedForScreenUpdate)
			{
				ChangeProjection();
				m_MarkedForScreenUpdate = false;
			}

			if (!m_Transform)
			{
				return;
			}

			const Vector3 camPos{ m_Transform->GetWorldPosition() };

			const Vector3 lookAt{ camPos + m_Transform->GetForward() };
			m_View = XMMatrixLookAtRH(camPos, lookAt, Vector3::Up);
			m_ViewInverse = XMMatrixInverse(nullptr, m_View);
			m_ViewProj = m_View * m_Proj;
			m_ViewProjInverse = XMMatrixInverse(nullptr, m_ViewProj);
		});
}

void CameraComponent::MoveCamera(const Vector3 pos)
{
	if(m_Transform)
	{
		m_CurrentPos += pos;
		m_Transform->Translate(m_CurrentPos);
	}
}

void CameraComponent::SetFOV(float fov)
{
	m_FOV = fov;
	m_MarkedForScreenUpdate = true;
}

void CameraComponent::OnScreensizeChanged(const int, const int)
{
	m_MarkedForScreenUpdate = true;
}
