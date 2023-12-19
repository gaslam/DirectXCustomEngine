#pragma once
#include "Components/MeshRenderComponent.h"
#include "SceneClasses/GameObject.h"

class Shape :
    public GameObject
{
public:
	explicit Shape(const std::wstring& texturePath = L"") :
	m_TexturePath{texturePath}
	{
		
	};
    virtual ~Shape() override = default;
protected:
    void Initialize(const SceneContext&) override;
    MeshRenderComponent* GetMesh() const { return m_pMesh; }
private:
    const std::wstring m_TexturePath;
    MeshRenderComponent* m_pMesh{};
};

