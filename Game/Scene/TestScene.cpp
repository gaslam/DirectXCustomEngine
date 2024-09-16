#include "TestScene.h"
#include "Prefabs/ModelObject.h"
#include "Prefabs/Shapes/Circle.h"

namespace Exe
{
	void TestScene::Initialize()
	{
		//m_pCircle = AddChild(new Circle{ 5,L"earth.bmp" });

		//AddUpdateCallback(
		//	[&](const Scene*)
		//	{
		//		auto timer{ Locator::GetTimer() };

		//		auto pTransform{ m_pCircle->GetTransform() };
		//		if (pTransform)
		//		{
		//			const double rotationSpeed{ 0.5f };
		//			const double elapsedTime{ timer->GetElapsedSeconds() };
		//			const double angle{ rotationSpeed * elapsedTime };
		//			const float angleFl{ static_cast<float>(angle) };
		//			pTransform->Rotate({ 0,angleFl,0 });
		//		}
		//	});
	}
		}
