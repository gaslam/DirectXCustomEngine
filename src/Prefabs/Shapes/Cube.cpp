#include "pch.h"
#include "Cube.h"

void Cube::SetDimensions(Vector3& dimensions)
{
	m_Dimensions = dimensions;
	MeshRenderComponent* pMesh{ GetMesh() };
	auto pBox{ GeometricPrimitive::CreateBox(dimensions) };
	pMesh->SetShape(pBox);
}

void Cube::SetDimensions(float width, float height, float depth)
{
	Vector3 dimensions{ width,height,depth };
	SetDimensions(dimensions);
}

Vector3 Cube::GetScale() const
{
	TransformComponent* pTransform{ GetTransform() };
	return pTransform->GetWorldScale();
}

void Cube::SetScale(Vector3& scale)
{
	TransformComponent* pTransform{ GetTransform() };
	pTransform->SetScale(scale);
}

void Cube::SetScale(float width, float height, float depth)
{
	Vector3 scale{ width,height,depth };
	SetScale(scale);
}

void Cube::Initialize(const SceneContext& context)
{
	Shape::Initialize(context);

	SetDimensions(m_Dimensions);
}
