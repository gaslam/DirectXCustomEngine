#include "pch.h"
#include "Scene.h"

void Engine::Scene::RootRender()
{
	Render();
	for (const auto& child : m_pChildren)
	{
		child->Render();
	}

	PostRender();

	for(const auto& child: m_pChildren)
	{
		child->PostRender();
	}
}

void Engine::Scene::RootInitialize()
{
	//Code to be added
}

void Engine::Scene::RootUpdate(float deltaTime)
{
	Update(deltaTime);

	for(const auto& child: m_pChildren)
	{
		child->Update(deltaTime);
	}
}

void Engine::Scene::AddChild(GameObject* object)
{

	m_pChildren.emplace_back(std::unique_ptr<GameObject>(object));

}

void Engine::Scene::RemoveChild(GameObject* object)
{
	const auto it{ std::find_if(m_pChildren.begin(),m_pChildren.end(),[object](const std::unique_ptr<GameObject>& other)
	{
		if (other.get() == object)
		{
			return true;
		}
		return false;
	}) };

	if(it == m_pChildren.end())
	{
		return;
	}
	m_pChildren.erase(it);
}