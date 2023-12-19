#include "pch.h"
#include "Shape.h"

void Shape::Initialize(const SceneContext&)
{
	m_pMesh = AddComponent<MeshRenderComponent>(m_TexturePath);
}
