#include "pch.h"
#include "Circle.h"
#include "Utils/GameHandler.h"
#include "Utils/Locator.h"
#include "Utils/Utils.h"
#include "SceneClasses/Scene.h"

void Circle::RenderImGui()
{
#ifdef _DEBUG
	const char* format{ "%.1f" };
	const float currentDiameter{ m_Diameter };
	float newDiameter{ currentDiameter };
	ImGui::SliderFloat("Diameter", &newDiameter, 1.f, 10.f, format,ImGuiSliderFlags_None);
	if(MathUtils::IsNotEqual(currentDiameter,newDiameter))
	{
		SetDiameter(newDiameter);
	}
#endif

}

void Circle::SetDiameter(float diameter)
{
	m_Diameter = diameter;
	m_IsMarkedForResize = true;
}


void Circle::ResizeSphere()
{
	if(!m_IsMarkedForResize)
	{
		return;
	}
	TransformComponent* pTransform{ GetTransform() };
	if (!pTransform)
	{
		return;
	}

	pTransform->SetScale(m_Diameter);

	const GameHandlerBase* pGameHandler{ Locator::GetGameHandler() };
	auto newSphere = GeometricPrimitive::CreateSphere(m_Diameter, 16, true, false, pGameHandler->GetDevice());
	m_pSphere.swap(newSphere);
	m_IsMarkedForResize = false;
}

void Circle::Initialize(Scene* pScene)
{
	Shape::Initialize(pScene);
	ChangeTag(L"Circle");
	SetDiameter(m_Diameter);

	const GameHandlerBase* pGameHandler{ Locator::GetGameHandler() };

	ShapeRenderComponent* pMesh{ GetMesh() };
	try {
		m_pSphere = GeometricPrimitive::CreateSphere(m_Diameter,16,true,false,pGameHandler->GetDevice());
		pMesh->SetShape(m_pSphere);
	}
	catch (const std::logic_error& e) {
		const auto wErrorMessage{ StringUtils::EncodeUTF8(e.what()) };
		Logger::LogError(wErrorMessage);
	}

	pScene->AddUpdateCallback([this](const Scene*)
		{
			ResizeSphere();
		});
}
