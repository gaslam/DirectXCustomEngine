#include "Shape.h"

void Shape::Initialize()
{
	m_pMesh = AddComponent<ShapeRenderComponent>(m_TexturePath);
}
