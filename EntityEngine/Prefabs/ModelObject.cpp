#include "ModelObject.h"

#include "Components/Meshes/ModelRenderComponent.h"

void ModelObject::Initialize(Scene*)
{
	m_pMesh = AddComponent<ModelRenderComponent>(m_ModelFilePath);
}
