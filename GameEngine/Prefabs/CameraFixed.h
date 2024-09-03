#pragma once
#include "SceneClasses/GameObject.h"
namespace Engine
{
    class CameraFixed :
        public GameObject
    {
    public:
        void Initialize(Scene*) override;
    };
}


