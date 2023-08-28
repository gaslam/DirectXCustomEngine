#pragma once
#include "Components/BaseComponent.h"
namespace Engine
{
    using namespace DirectX::SimpleMath;
    class CameraComponent :
        public BaseComponent
    {
    public:
        CameraComponent(DirectX::XMVECTORF32& startPos, const float rotationSpeed = 0.1f);
        void Initialize() override;
    private:
        float m_Pitch{};
        float m_Yaw{};

        Matrix m_View{};
        Matrix m_Proj{};
        Vector3 m_Pos{};
        Color m_RoomColor{};

        //const DirectX::XMVECTORF32 START_POSITION = { 0.f, -1.5f, 0.f, 0.f };
        const DirectX::XMVECTORF32 ROOM_BOUNDS = { 8.f, 6.f, 12.f, 0.f };
       // const DirectX::XMVECTORF32 ROOM_BOUNDS = { 8.f, 6.f, 12.f, 0.f };
	float m_RotSpeed{};
    };
}


