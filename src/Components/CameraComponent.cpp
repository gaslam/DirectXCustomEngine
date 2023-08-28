#include "pch.h"
#include "CameraComponent.h"

Engine::CameraComponent::CameraComponent(DirectX::XMVECTORF32& startPos, const float rotationSpeed) :
m_Pos{startPos},
m_RotSpeed{rotationSpeed}
{
}

void Engine::CameraComponent::Initialize()
{
}
