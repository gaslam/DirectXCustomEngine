#pragma once
#include "SceneClasses/GameObject.h"
namespace EntityEngine
{
    class CameraFixed :
        public GameObject
    {
    public:

        //TODO: Fix warnings that are caused by the default constructor being deleted in base class. For now this works
        ~CameraFixed() override = default;

        CameraFixed(const CameraFixed& other) = delete;
        CameraFixed(CameraFixed&& other) noexcept = delete;
        CameraFixed& operator=(const CameraFixed& other) = delete;
        CameraFixed& operator=(CameraFixed&& other) noexcept = delete;
        void Initialize(Scene*) override;
    };
}


