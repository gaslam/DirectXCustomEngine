#include "pch.h"
#include "Scene.h"

#include "Prefabs/CameraFixed.h"
#include "Components/CameraComponent.h"
#include "Prefabs/FreeMoveCamera.h"

using namespace Engine;

void Scene::RootOnScreensizeChanged(const int width, const int height)
{
	for (const auto& child : m_pChildren)
	{
		child->RootOnScreensizeChanged(width, height);
	}
}

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
	m_SceneContext.timer = gameContext.timer;

	//Code to be added
	Initialize();
	Scene::Initialize();

	FreeMoveCamera* pCamera{ new FreeMoveCamera{} };
	AddChild(pCamera);

	m_pActiveCamera = pCamera->GetComponent<CameraComponent>();

	m_SceneContext.pCameraComp = m_pActiveCamera;
	m_pDefaultCamera = m_pActiveCamera;
}

void Scene::RootUpdate()
{
	Update(m_SceneContext);

	for(const auto& child: m_pChildren)
	{
		child->RootUpdate(m_SceneContext);
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

std::unique_ptr<GameObject> Scene::RemoveChild(GameObject* object, bool keepActive)
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
		return nullptr;
	}

	std::unique_ptr<GameObject> newObject{ nullptr };

	if(keepActive)
	{
		newObject = std::move(*it);
		;
	}

	m_pChildren.erase(it);

	return std::move(newObject);
}
