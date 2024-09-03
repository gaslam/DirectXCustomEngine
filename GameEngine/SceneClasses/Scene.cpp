#include "pch.h"
#include "Scene.h"

#include <Prefabs/CameraFixed.h>


#include "imgui.h"

#include "Components/CameraComponent.h"
#include "Prefabs/FreeMoveCamera.h"
#include "Utils/Utils.h"

using namespace Engine;

void Scene::RootOnScreensizeChanged(const int width, const int height)
{

	for(const std::unique_ptr<GameObject>& pChild: m_pChildren)
	{
		pChild->RootOnScreensizeChanged(width, height);
	}
}

void Scene::RootRender()
{
	Render();
	for (const std::unique_ptr<GameObject>& pChild : m_pChildren)
	{
		pChild->Render();
	}

	PostRender();
	for (const std::unique_ptr<GameObject>& pChild : m_pChildren)
	{
		pChild->PostRender();
	}
}

void Scene::RootRenderImGui() const
{

	for (const std::unique_ptr<GameObject>& pChild : m_pChildren)
	{
		pChild->RootRenderImGui();
	}
}

void Scene::RootInitialize()
{
	//Code to be added
	Initialize();
	Scene::Initialize();

	FreeMoveCamera* pCamera{ new FreeMoveCamera{} };
	AddChild(pCamera);

	m_pActiveCamera = pCamera->GetComponent<CameraComponent>();

	m_pDefaultCamera = m_pActiveCamera;
}

void Scene::RootInitDeviceResources() const
{
	for(const std::unique_ptr<GameObject>& pChild: m_pChildren)
	{
		pChild->RootInitDeviceResources();
	}
}

void Scene::RootOnDeviceLost()
{
	for (const std::unique_ptr<GameObject>& pChild : m_pChildren)
	{
		pChild->OnDeviceLost();
	}
}

void Scene::Initialize()
{
	for (const std::unique_ptr<GameObject>& pChild : m_pChildren)
	{
		pChild->RootInitialize(this);
	}
}
