#pragma once
#include "pch.h"
#include <Components/ComponentPool.h>

class Registry final {
public:

	template
	<typename T>
	void RegisterComponent(Entity entity) {
		const auto pool{ GetComponentPool<T>() };

		if (const auto poolCasted = static_cast<ComponentPool<T>*>(pool))
		{
			poolCasted->AddComponent(entity);
		}
		Logger::LogWarning(L"Component not found in pool");
	}

	template
	<typename T>
	[[nodiscard]] IComponentPool* GetComponentPool() {
		const char* componentName{ typeid(T).name()};
		const auto found{ std::find_if(m_ComponentPools.begin(), m_ComponentPools.end(), [componentName](const std::pair<const char*, std::unique_ptr<IComponentPool>>& pair) {
			return strcmp(pair.first, componentName) == 0;
			}) };

		if (found != m_ComponentPools.end()) {
			return found->second.get();
		}
		return m_ComponentPools.emplace_back(componentName, std::make_unique<ComponentPool<T>>()).second.get();
	}

	template
	<typename T>
	[[nodiscard]] IComponentPool* GetComponentPool(const char* componentName) {
		const auto found{ std::find_if(m_ComponentPools.begin(), m_ComponentPools.end(), [componentName](const std::pair<const char*, std::unique_ptr<IComponentPool>>& pair) {
			return strcmp(pair.first, componentName) == 0;
			}) };

		if (found != m_ComponentPools.end()) {
			return found->second.get();
		}
		return m_ComponentPools.emplace_back(componentName, std::make_unique<ComponentPool<T>>()).second.get();
	}

	[[nodiscard]] std::vector<std::pair<const char*, std::unique_ptr<IComponentPool>>> GetComponentPools() const {
		return m_ComponentPools;
	}

private:
	std::vector<std::pair<const char*, std::unique_ptr<IComponentPool>>> m_ComponentPools;
};