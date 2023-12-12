#include "pch.h"
#include "CameraComponent.h"

#include "SceneClasses/GameObject.h"
#include "Components/TransformComponent.h"

using namespace Engine;

void CameraComponent::Update(const SceneContext& context)
{
	if(m_MarkedForScreenUpdate)
	{
		ChangeProjection(context);
		m_MarkedForScreenUpdate = false;
	}

	if(!m_Transform)
	{
		return;
	}

	//const float elapsedTime{ static_cast<float>(context.timer->GetElapsedSeconds()) };

//	m_Pitch += elapsedTime * 10.f;

	const Vector3 camPos{ m_Transform->GetWorldPosition() };
	m_Transform->Translate(camPos);
	m_Transform->Rotate(m_Roll,m_Pitch,m_Yaw);

	const Vector3 lookAt{ camPos + m_Transform->GetForward() };
	m_View = XMMatrixLookAtRH(camPos, lookAt, Vector3::Up);
	m_ViewInverse = XMMatrixInverse(nullptr, m_View);
	m_ViewProj = m_View * m_Proj;
	m_ViewProjInverse = XMMatrixInverse(nullptr, m_ViewProj);
}

void CameraComponent::ChangeProjection(const SceneContext& context)
{
	const auto size{ context.d12Context.pDeviceResources->GetOutputSize() };
	m_Proj = Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(m_FOV),
		static_cast<float>(size.right) / static_cast<float>(size.bottom), 0.01f, 100.f);
}

void CameraComponent::Initialize(const SceneContext&)
{
	constexpr Vector3 startPos{ 0.f, -1.5f, 0.f };
	const GameObject* owner{ GetOwner() };

	if(owner == nullptr)
	{
		Logger::LogWarning(L"Cannot get transform on camera!!");
		return;
	}

	m_Transform = owner->GetTransform();
	m_Transform->Translate(startPos);
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
