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

    //TODO: Fix warnings that are caused by the default constructor being deleted in base class. For now this works
	~ModelObject() override = default;
    ModelObject(const ModelObject& other) = delete;
    ModelObject(ModelObject&& other) noexcept = delete;
    ModelObject& operator=(const ModelObject& other) = delete;
    ModelObject& operator=(ModelObject&& other) noexcept = delete;
protected:
    void Initialize(Scene*) override;
    ModelRenderComponent* GetMesh() const { return m_pMesh; }
private:
    const std::wstring m_ModelFilePath{};
    ModelRenderComponent* m_pMesh{};
};

