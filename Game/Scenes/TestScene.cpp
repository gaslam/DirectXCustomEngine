#include "pch.h"
#include "TestScene.h"

#include "imgui_internal.h"
#include "Prefabs/ModelObject.h"

using namespace Engine;
using namespace SimpleMath;

void TestScene::Initialize()
{
	m_Model = AddChild(new ModelObject{L"Models/cup.sdkmesh" });

}

void TestScene::Update()
{
	const auto timer{ Locator::GetTimer() };
	const float elapsedTime{ static_cast<float>(timer->GetElapsedSeconds()) };
	const auto pTransform{ m_Model->GetTransform() };
	m_Rotation.y += XMConvertToRadians(m_RotSpeedDeg) * elapsedTime;
	pTransform->Rotate(m_Rotation);
}

void TestScene::RenderImGui()
{
	ImGui::SeparatorText("Variables:");

	const char* stringFormat{ "%.1f" };
	ImGui::SliderFloat("Rotation speed (in degrees)", &m_RotSpeedDeg, 0.f, 360.f, stringFormat);
}
