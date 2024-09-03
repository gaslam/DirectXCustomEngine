#include "Shape.h"

void Shape::Initialize(Scene*)
{
	m_pMesh = AddComponent<ShapeRenderComponent>(m_TexturePath);
}
