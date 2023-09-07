#pragma once
#include "Misc/Singleton.h"
#include "SceneClasses/Scene.h"
#include <vector>
namespace Engine
{
	class SceneManager : public Singleton<SceneManager>
	{
	public:
		void Render() const;
		void Update(float deltaTime) const;
		template
		<class T>
		std::enable_if_t<std::is_base_of_v<Scene,T>,T*> AddScene(std::unique_ptr<T> scene)
		{
			auto scenePointer{ scene.get() };
			const bool alreadyAdded{ std::any_of(m_pScenes.begin(),m_pScenes.end(),[scenePointer](const std::unique_ptr<Scene>& pScene)
		{
			return pScene->GetName() == scenePointer->GetName();
		}) };

			if (alreadyAdded)
			{
				return nullptr;
			}
			m_pScenes.emplace_back(std::move(scene));
			scenePointer->RootInitialize();
			return scenePointer;
		}
		void Initialize();
		void RemoveScene(Scene* scene);
		void SetScene(const std::wstring& name);
		void SetScene(UINT id);
		void ChangeScene(Scene* scene);
		void GoToNextScene();
		void GoToPreviousScene();
	private:
		std::vector<std::unique_ptr<Scene>> m_pScenes{};
		Scene* m_pActiveScene{};
	};

}
