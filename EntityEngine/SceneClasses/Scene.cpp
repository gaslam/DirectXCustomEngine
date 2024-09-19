#include "pch.h"
#include "Scene.h"

using namespace EntityEngine;

void Scene::RootOnScreensizeChanged(const int width, const int height)
{

	//for(const std::unique_ptr<GameObject>& pChild: m_pChildren)
	//{
	//	pChild->RootOnScreensizeChanged(width, height);
	//}
}

void Scene::RootRender()
{
	for (const auto& callback : m_pRenderCallbacks)
	{
		callback(this);
	}
}

void Scene::RootRenderImGui() const
{

	//for (const std::unique_ptr<GameObject>& pChild : m_pChildren)
	//{
	//	pChild->RootRenderImGui();
	//}
}

void Scene::RootInitialize()
{
//	//Code to be added
	Initialize();
    Scene::Initialize();
}

void Scene::RootInitDeviceResources() const
{
//	for(const std::unique_ptr<GameObject>& pChild: m_pChildren)
//	{
//		pChild->RootInitDeviceResources();
//	}
}

void Scene::RootOnDeviceLost()
{
//	for (const std::unique_ptr<GameObject>& pChild : m_pChildren)
//	{
//		pChild->OnDeviceLost();
//	}
}

void Scene::Initialize()
{
	for (const auto& callback: m_pInitializerCallbacks)
	{
		callback(this);
	}
}
