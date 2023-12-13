#pragma once
#include "Components/MeshRenderComponent.h"
#include "SceneClasses/GameObject.h"

class Shape :
    public GameObject
{
public:
	Shape() = default;
    virtual ~Shape() override = default;
protected:
    void Initialize(const SceneContext&) override;
    MeshRenderComponent* GetMesh() const { return m_pMesh; }
private:
    MeshRenderComponent* m_pMesh{};
};

