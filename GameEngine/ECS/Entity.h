#pragma once
#include <array>

#include "Components/TransformComponentECS.h"
#include "Main/Logger.h"
using Entity = std::size_t;

constexpr Entity EntityMax = 32;

inline Entity CreateEntity() noexcept
{
	static Entity entities{ 0 };

	if(entities >= EntityMax)
	{
		Logger::LogError(L"Entity count bigger or equal to max entities");
		return 0;
	}
	return entities++;
}

struct Registry
{
	std::array<Engine::TransformComponentECS,EntityMax> pTransforms{};
};