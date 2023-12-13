#include "pch.h"
#include "TestScene.h"

#include "Components/MeshRenderComponent.h"
#include "Managers/InputManager.h"
#include "Prefabs/Shapes/Circle.h"

using namespace Engine;

void TestScene::Initialize()
{
	const float radius{ 10.f };
	m_Circle = AddChild(new Circle{radius});


}

void TestScene::Update(const SceneContext& context)
{
	const float elapsedSec{ static_cast<float>(context.timer->GetElapsedSeconds()) };
	float radius = m_Circle->GetDiameter();
	if(radius >= m_MaxScale)
	{
		radius = m_MaxScale;
		m_ScaleSpeed = -m_ScaleSpeed;
	}

	if (radius <= m_MinScale)
	{
		radius = m_MinScale;
		m_ScaleSpeed = -m_ScaleSpeed;
	}

	radius += elapsedSec * m_ScaleSpeed;
	m_Circle->SetDiameter(radius);
}
