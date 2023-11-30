#include "pch.h"
#include "CameraFixed.h"

#include "Components/CameraComponent.h"

using namespace Engine;

void CameraFixed::Initialize(const SceneContext&)
{
	AddComponent<CameraComponent>();
}
