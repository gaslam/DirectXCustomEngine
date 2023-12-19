#include "pch.h"
#include "GameObject.h"

#include "Scene.h"

void GameObject::RootSceneAttach(Scene* pScene)
{
	if(!pScene)
	{
		const auto pLogger{ Logger::GetInstance() };
		pLogger->LogError(L"Cannot attach scene to GameObject!!");
	}

	m_pParentScene = pScene;

	const SceneContext pContext{ m_pParentScene->GetSceneContext() };

	RootInitialize(pContext);

	OnSceneAttach(m_pParentScene);

}
