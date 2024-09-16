#pragma once
#include "MeshRenderComponent.h"
class ModelRenderComponent :
    public MeshRenderComponent
{
public:
    explicit ModelRenderComponent(const std::wstring& modelFilePath) : MeshRenderComponent(modelFilePath){}

    //TODO: Fix warnings that are caused by the default constructor being deleted in base class. For now this works

    ModelRenderComponent(const ModelRenderComponent& other) = delete;
    ModelRenderComponent(ModelRenderComponent&& other) noexcept = delete;
    ModelRenderComponent& operator=(const ModelRenderComponent& other) = delete;
    ModelRenderComponent& operator=(ModelRenderComponent&& other) noexcept = delete;

    void InitDeviceResources() override;
	void Initialize(Scene* pScene) override;
    void OnDeviceLost() override;
private:
    std::unique_ptr<CommonStates> m_pStates;
    std::unique_ptr<EffectFactory> m_pFxFactory;
    std::unique_ptr<EffectTextureFactory> m_pModelResources;
    std::unique_ptr<Model> m_pModel;
    Model::EffectCollection m_ModelNormal;
};

