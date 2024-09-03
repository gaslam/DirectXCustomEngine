#include "CameraFixed.h"

#include "Components/CameraComponent.h"

using namespace Engine;

void CameraFixed::Initialize(Scene*)
{
	AddComponent<CameraComponent>();
	ChangeTag(L"FixedCamera");
}
