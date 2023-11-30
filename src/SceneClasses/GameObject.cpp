#include "pch.h"
#include "GameObject.h"

void Engine::GameObject::RootSceneAttach(Scene* pScene)
{
	if(!pScene)
	{
		const auto pLogger{ Logger::GetInstance() };
		pLogger->LogError(L"Cannot attach scene to GameObject!!");
	}

	m_pParentScene = pScene;

}
