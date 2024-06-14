#pragma once
#include "ECS/Entity.h"

struct TransformSystem
{
	constexpr float deltaTime{ 0.1f };
	constexpr float gravity{ -9.81f };

	constexpr float velocity{ gravity * (deltaTime * deltaTime) };
	void Update(Registry& registry) const
	{
		for(int i{}; i <= EntityMax; ++i)
		{
			if(registry.pTransforms.contains(i))
			{
				registry.pTransforms[i].m_WorldPosition.y += gravity;
			}
		}
	}
};
