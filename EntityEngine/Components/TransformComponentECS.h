#pragma once

using namespace DirectX;
using namespace SimpleMath;

namespace EntityEngine
{
	class TransformComponentECS final
	{
		Vector3 m_WorldPosition{};
		Vector3 m_WorldScale{ 1.f,1.f,1.f };
		Vector3 m_Forward{ Vector3::Forward };
		Vector4 m_WorldRotation{};
		Vector4 m_LocalRotation{ m_WorldRotation };
	};
}