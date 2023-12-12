#include "pch.h"
#include "SceneManager.h"

using namespace Engine;

void SceneManager::Render() const
{
	if (m_pActiveScene)
	{
		m_pActiveScene->RootRender();
	}
}

void SceneManager::Update() const
{
	if (m_pActiveScene)
	{
		m_pActiveScene->RootUpdate();
	}
}

void SceneManager::Initialize(GameContext& gameContext) const
{
	for(const auto& scene: m_pScenes)
	{
		scene->RootInitialize(gameContext);
	}
}

void SceneManager::RemoveScene(Scene* scene)
{
	const auto it{ std::remove_if(m_pScenes.begin(),m_pScenes.end(),[scene](const std::unique_ptr<Scene>& pScene)
{
	return pScene->GetName() == scene->GetName();
}) };

	if (it == m_pScenes.end())
	{
		return;
	}
	m_pScenes.erase(it);
}

void SceneManager::SetScene(const std::wstring& name)
{
	const auto it{ std::find_if(m_pScenes.begin(),m_pScenes.end(),[name](const std::unique_ptr<Scene>& pScene)
	{
		return pScene->GetName() == name;
	}) };

	if (it != m_pScenes.end())
	{
		return;
	}

	const auto scene{ it->get() };
	ChangeScene(scene);
}

void SceneManager::SetScene(UINT id)
{
	const auto scenesSize{ m_pScenes.size() };
	if(id >= scenesSize)
	{
		return;
	}

	const auto newScene{ m_pScenes[id].get() };
	ChangeScene(newScene);
}

void SceneManager::ChangeScene(Scene* scene)
{
	if(m_pActiveScene)
	{
		m_pActiveScene->OnSceneDeactivated();
	}
	m_pActiveScene = scene;
	m_pActiveScene->OnSceneActivated();
}

void SceneManager::GoToNextScene()
{
	const size_t sceneSize{ m_pScenes.size() };
	for (size_t i{}; i < sceneSize; ++i)
	{
		const auto pScene{ m_pScenes[i].get() };
		if (pScene == m_pActiveScene)
		{
			size_t nextIndex{ i + 1 };

			if(nextIndex >= sceneSize)
			{
				nextIndex = 0;
			}

			const auto newScene{ m_pScenes[nextIndex].get() };
			ChangeScene(newScene);
		}
	}
}

void SceneManager::GoToPreviousScene()
{
	const size_t sceneSize{ m_pScenes.size() };
	for (size_t i{}; i < sceneSize; ++i)
	{
		if (i == 0)
		{
			i = sceneSize;
		}
		const auto newScene{ m_pScenes[i].get() };
		ChangeScene(newScene);
	}
}

void SceneManager::OnDeviceLost()
{
	if(m_pActiveScene)
	{
		m_pActiveScene->RootOnDeviceLost();
	}
}

void SceneManager::OnScreensizeChanged(const int width, const int height)
{
	if (m_pActiveScene)
	{
		m_pActiveScene->RootOnScreensizeChanged(width,height);
	}
}
