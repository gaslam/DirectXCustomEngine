#pragma once
#include "MeshRenderComponent.h"
class ShapeRenderComponent final :
    public MeshRenderComponent
{
public:
    explicit ShapeRenderComponent(const std::wstring& dir = L"") : MeshRenderComponent(dir) {};
    void SetShape(std::unique_ptr<GeometricPrimitive>& shape);
    void Render() override;
    void InitDeviceResources() override;
    void OnDeviceLost() override;

    template
        <typename T>
        std::enable_if_t<std::is_base_of_v<DirectX::BasicEffect, T>, T*>AddEffect(T* pEffect)
    {
        m_pEffects.emplace_back(std::unique_ptr<DirectX::BasicEffect>(pEffect));
        return pEffect;
    }

private:
    std::unique_ptr<DescriptorHeap> m_pResourceDescriptors;
    std::vector<std::unique_ptr<DirectX::BasicEffect>> m_pEffects;
    std::unique_ptr<GeometricPrimitive> m_pShape{};

    enum Descriptors
    {
        Earth,
        Count
    };
};

