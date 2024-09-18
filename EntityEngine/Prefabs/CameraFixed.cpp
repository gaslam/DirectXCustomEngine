#include "CameraFixed.h"

#include "Components/CameraComponent.h"

using namespace EntityEngine;

void CameraFixed::Initialize(Scene*)
{
	AddComponent<CameraComponent>();
	ChangeTag(L"FixedCamera");
}
