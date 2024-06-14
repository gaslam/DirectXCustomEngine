#include "ModelObject.h"

#include "Components/Meshes/ModelRenderComponent.h"

void ModelObject::Initialize()
{
	m_pMesh = AddComponent<ModelRenderComponent>(m_ModelFilePath);
}
