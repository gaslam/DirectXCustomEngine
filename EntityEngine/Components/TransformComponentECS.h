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

	public:
		void SetWorldPosition(const Vector3& worldPosition)
		{
			m_WorldPosition = worldPosition;
		}

		[[nodiscard]] const Vector3& GetWorldPosition() const
		{
			return m_WorldPosition;
		}

		void SetForward(const Vector3& forward) 
		{
			m_Forward = forward;
		}

		[[nodiscard]] const Vector3& GetForward() const
		{
			return m_Forward;
		}

		void Move(const Vector3& direction)
		{
			m_WorldPosition += direction;
		}
	};
}