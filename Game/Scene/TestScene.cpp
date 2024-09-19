#include "TestScene.h"

#include "Components/TransformComponentECS.h"

namespace Exe
{
	void TestScene::Initialize()
	{

		AddInitializerCallback(
			[](const Scene* scene)
			{
				const auto registry{ scene->GetRegistry() };
				constexpr Vector3 forward{ 1.f,1.f,0.f };
				for (unsigned int i = 0; i < 100000; i++)
				{
					const Entity entity{ registry->CreateEntity() };
					const auto pComp{ registry->RegisterComponent<TransformComponentECS>(entity) };
					pComp->SetForward(forward);
				}
			});

		AddUpdateCallback({
			[this](const Scene* scene)
			{
				const auto registry{ scene->GetRegistry() };
				const auto pool{ static_cast<ComponentPool<TransformComponentECS>*>(registry->GetComponentPool<TransformComponentECS>()) };

				for (size_t i = 0; i < pool->GetComponentCount(); i++)
				{
					auto& transformComponent{ pool->GetComponent(static_cast<Entity>(i)) };
					auto& pos{ transformComponent.GetWorldPosition() };
					auto forward{ transformComponent.GetForward() };
					if (pos.x >= m_Bounds.x || pos.x <= -m_Bounds.x)
					{
						forward.x *= -1;
					}

					if (pos.y >= m_Bounds.x || pos.y <= -m_Bounds.y)
					{
						forward.y *= -1;
					}
					transformComponent.SetForward(forward);
					const Vector3 dir{ forward.x * 10.f, forward.y * 10.f,0.f };
					transformComponent.Move(dir);
				}
			}
			});
	}
}
