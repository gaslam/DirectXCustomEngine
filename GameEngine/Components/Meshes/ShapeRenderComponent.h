#pragma once
#include "MeshRenderComponent.h"
class ShapeRenderComponent final :
    public MeshRenderComponent
{
public:
    explicit ShapeRenderComponent(const std::wstring& dir = L"") : MeshRenderComponent(dir) {};
    //TODO: Fix warnings that are caused by the default constructor being deleted in base class. For now this works
    ~ShapeRenderComponent() override = default;

    ShapeRenderComponent(const ShapeRenderComponent& other) = delete;
    ShapeRenderComponent(ShapeRenderComponent&& other) noexcept = delete;
    ShapeRenderComponent& operator=(const ShapeRenderComponent& other) = delete;
    ShapeRenderComponent& operator=(ShapeRenderComponent&& other) noexcept = delete;

    void SetShape(std::unique_ptr<GeometricPrimitive>& shape);
	void Initialize(Scene* pScene) override;
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

