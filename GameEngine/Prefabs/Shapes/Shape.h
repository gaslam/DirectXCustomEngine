#pragma once
#include "Components/Meshes/ShapeRenderComponent.h"
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
    void Initialize() override;
    ShapeRenderComponent* GetMesh() const { return m_pMesh; }
    virtual void RenderImGui() override{}
private:
    const std::wstring m_TexturePath{};
    ShapeRenderComponent* m_pMesh{};
};

