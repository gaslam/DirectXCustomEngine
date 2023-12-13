#include "pch.h"
#include "Circle.h"

void Circle::SetDiameter(float diameter)
{
	TransformComponent* pTransform{ GetTransform() };
	if (!pTransform)
	{
		return;
	}
	m_Diameter = diameter;
	Vector3 radiusScale{pTransform->GetWorldScale()};
	radiusScale.x = diameter;
	radiusScale.y = diameter;
	radiusScale.z = diameter;
	pTransform->SetScale(radiusScale);
}

void Circle::Initialize(const SceneContext&context)
{
	Shape::Initialize(context);
	SetDiameter(m_Diameter);

	MeshRenderComponent* pMesh{ GetMesh() };
	auto pSphere{ GeometricPrimitive::CreateSphere(m_Diameter) };
	pMesh->SetShape(pSphere);
}
