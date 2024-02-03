#pragma once
#include <bitset>

class Component;
class Entity;

using ComponentTypeID = std::size_t;

inline ComponentTypeID GetComponentTypeID()
{
	static ComponentTypeID lastID = 0;
	return ++lastID;
}

template <typename T> inline ComponentTypeID GetComponentTypeID() noexcept
{
	static ComponentTypeID typeID{ GetComponentTypeID() };
	return typeID;
}


constexpr std::size_t maxComponents{ 32 };

using ComponentBitSet = bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;


