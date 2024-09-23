#pragma once
#include "pch.h"
#include <Components/ComponentPool.h>

class Registry final {
public:

	template
	<typename T,typename... Args>
	T* RegisterComponent(Entity entity,Args&&... args) {
		const auto pool{ GetComponentPool<T>() };

		if (const auto poolCasted = static_cast<ComponentPool<T>*>(pool))
		{
			return &poolCasted->AddComponent(entity,std::forward<Args>(args)...);
		}
		Logger::LogWarning(L"Component not found in pool");
		return nullptr;
	}


	[[nodiscard]] Entity CreateEntity() {
		return m_EntityCounter++;
	}

	[[nodiscard]] Entity GetEntityCount() const {
		return m_EntityCounter;
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

	template
	<typename T>
	void RemoveComponentEntity(Entity entity) {
		auto pool{ GetComponentPool<T>() };

		if (const auto poolCasted = static_cast<ComponentPool<T>*>(pool))
		{
			poolCasted->Remove(entity);
		}
	}

	template
		<typename T>
		void GetComponent(Entity entity) {
		auto pool{ GetComponentPool<T>() };

		if (const auto poolCasted = static_cast<ComponentPool<T>*>(pool))
		{
			poolCasted->GetComponent(entity);
		}
	}

	template
<typename T>
	void RemoveComponent()
	{
		auto pool{ GetComponentPool<T>() };

		if (const auto poolCasted = static_cast<ComponentPool<T>*>(pool.get())) {

			m_EntityCounter -= poolCasted->GetEntityCount();
			// Clear the component pool
			poolCasted->Clear();

			// Remove the component pool entry from m_ComponentPools
			const char* componentName = typeid(T).name();
			m_ComponentPools.erase(std::remove_if(m_ComponentPools.begin(), m_ComponentPools.end(),
				[componentName](const std::pair<const char*, std::unique_ptr<IComponentPool>>& pair) {
					return strcmp(pair.first, componentName) == 0;
				}), m_ComponentPools.end());
		}
	}

	template
		<typename T>
		[[nodiscard]] bool HasComponent() const
	{
		const char* componentName{ typeid(T).name() };
		return std::any_of(m_ComponentPools.begin(), m_ComponentPools.end(), [componentName](const std::pair<const char*, std::unique_ptr<IComponentPool>>& pair) {
			return strcmp(pair.first, componentName) == 0;
			});
	}

private:

	Entity m_EntityCounter{ 0 };
	std::vector<std::pair<const char*, std::unique_ptr<IComponentPool>>> m_ComponentPools;
};