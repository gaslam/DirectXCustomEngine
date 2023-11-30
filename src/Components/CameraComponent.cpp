#include "pch.h"
#include "CameraComponent.h"

using namespace Engine;

void CameraComponent::Update(const SceneContext& context)
{
	auto size{ context.d12Context.pDeviceResources->GetOutputSize() };
	m_proj = Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(70.f),
   static_cast<float>(size.right) / static_cast<float>(size.bottom), 0.01f, 100.f);
	float y = sinf(m_pitch);
	float r = cosf(m_pitch);
	float z = r * cosf(m_yaw);
	float x = r * sinf(m_yaw);
	XMVECTOR lookAt = m_cameraPos + Vector3(x, y, z);
	m_view = XMMatrixLookAtRH(m_cameraPos, lookAt, Vector3::Up);
}

void CameraComponent::Initialize(const SceneContext&)
{
	constexpr  XMVECTORF32 startPos{ 0.f, -1.5f, 0.f, 0.f };
	m_cameraPos = startPos;
}
