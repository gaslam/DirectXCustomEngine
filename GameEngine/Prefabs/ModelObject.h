#pragma once
#include "SceneClasses/GameObject.h"

class ModelRenderComponent;
class ModelObject final :
    public GameObject
{
public:
    explicit ModelObject(const std::wstring& modelFilePath = L"") :
        m_ModelFilePath{ modelFilePath }
    {

    };
protected:
    void Initialize() override;
    ModelRenderComponent* GetMesh() const { return m_pMesh; }
private:
    const std::wstring m_ModelFilePath{};
    ModelRenderComponent* m_pMesh{};
};

