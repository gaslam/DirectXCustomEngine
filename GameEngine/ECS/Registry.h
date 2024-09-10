#pragma once
#include "pch.h"
#include <Components/ComponentPool.h>

class Registry final {
public:
private:
	std::vector<TPair<const char*, std::unique_ptr<IComponentPool>>> m_Components;
};