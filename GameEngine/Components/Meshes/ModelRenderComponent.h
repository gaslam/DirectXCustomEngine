#pragma once
#include "MeshRenderComponent.h"
class ModelRenderComponent :
    public MeshRenderComponent
{
public:
    explicit ModelRenderComponent(const std::wstring& modelFilePath) : MeshRenderComponent(modelFilePath){}
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

