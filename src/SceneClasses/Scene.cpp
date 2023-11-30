#include "pch.h"
#include "Scene.h"

#include "Components/CameraFixed.h"
#include "Components/CameraComponent.h"

using namespace Engine;

void Scene::RootRender()
{
	Render();
	for (const auto& child : m_pChildren)
	{
		child->Render(m_SceneContext);
	}

	PostRender();

	for(const auto& child: m_pChildren)
	{
		child->PostRender();
	}
}

void Scene::RootInitialize(GameContext& gameContext)
{
	m_SceneContext.d12Context = gameContext.d12Context;
	m_SceneContext.windowHeight = gameContext.windowHeight;
	m_SceneContext.windowWidth = gameContext.windowWidth;

	CameraFixed* pCamera{ new CameraFixed{} };
	AddChild(pCamera);

	//Code to be added
	Initialize();
	Scene::Initialize();

	m_pActiveCamera = pCamera->GetComponent<CameraComponent>();

	m_SceneContext.pCameraComp = m_pActiveCamera;
	m_pDefaultCamera = m_pActiveCamera;
}

void Scene::RootUpdate()
{
	Update(m_SceneContext);

	for(const auto& child: m_pChildren)
	{
		child->Update(m_SceneContext);
	}
}

void Scene::RootOnDeviceLost()
{
	for (const auto& child : m_pChildren)
	{
		child->OnDeviceLost();
	}
}

void Scene::Initialize()
{
	for(const auto& child: m_pChildren)
	{
		child->RootInitialize(m_SceneContext);
	}
}

GameObject* Scene::AddChild(GameObject* object)
{
	object->RootSceneAttach(this);
	m_pChildren.emplace_back(std::unique_ptr<GameObject>(object));
	return object;

}

void Scene::RemoveChild(GameObject* object)
{
	const auto it{ std::ranges::find_if(m_pChildren.begin(),m_pChildren.end(),[object](const std::unique_ptr<GameObject>& other)
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
