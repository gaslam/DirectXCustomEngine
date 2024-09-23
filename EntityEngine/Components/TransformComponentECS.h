#pragma once

using namespace DirectX;
using namespace SimpleMath;

namespace EntityEngine
{
	class TransformComponentECS final
	{
		bool m_RecalculateTransform{ true };
		Vector3 m_WorldPosition{};
		Vector3 m_WorldScale{ 1.f,1.f,1.f };
		Vector3 m_Forward{ Vector3::Forward };
		Vector3 m_Right{Vector3::Right};
		Vector3 m_LocalScale{ m_WorldScale };
		Vector4 m_WorldRotation{};
		Vector4 m_LocalRotation{ m_WorldRotation };

		XMMATRIX m_WorldMatrix{};


		void RecalculateTransform()
		{
			m_WorldRotation = m_LocalRotation;
			m_WorldScale = m_LocalScale;
			const auto rotMat = XMMatrixRotationQuaternion(m_WorldRotation);
			m_Forward = XMVector3TransformCoord(XMVectorSet(0.f, 0.f, 1.f, 0.f), rotMat);
			m_Right = XMVector3TransformCoord(XMVectorSet(-1.f, 0.f, 0.f, 0.f), rotMat);
			m_WorldMatrix = XMMatrixScaling(m_WorldScale.x, m_WorldScale.y, m_WorldScale.z) * XMMatrixRotationQuaternion(m_WorldRotation) * XMMatrixTranslation(m_WorldPosition.x, m_WorldPosition.y, m_WorldPosition.z);;

		}

	public:

		TransformComponentECS(const Vector3& worldPos = {}, const Vector3& rotation = {}, const Vector3& scale = {1.f,1.f,1.f},const Vector3& forward = {}) :
			m_WorldPosition(worldPos),
			m_WorldScale(scale),
			m_Forward(forward),
			m_WorldRotation(rotation)
		{
		}

		void SetForward(const Vector3& forward)
		{
			m_Forward = forward;
			m_Forward.Normalize();
			m_RecalculateTransform = true;
		}

		void SetWorldPosition(const Vector3& worldPosition)
		{
			m_WorldPosition = worldPosition;
		}

		[[nodiscard]] const Vector3& GetWorldPosition()
		{
			if (m_RecalculateTransform)
			{
				RecalculateTransform();
			}
			return m_WorldPosition;
		}

		[[nodiscard]] const Vector3 GetForward()
		{
			if (m_RecalculateTransform)
			{
				RecalculateTransform();
			}
			return m_Forward;
		}

		[[nodiscard]] Vector3 GetRight()
		{
			if (m_RecalculateTransform)
			{
				RecalculateTransform();
			}
			return m_Right;
		}

		[[nodiscard]] const Vector4& GetWorldRotation()
		{
			if (m_RecalculateTransform)
			{
				RecalculateTransform();
			}
			return m_WorldRotation;
		}

		[[nodiscard]] const XMMATRIX& GetWorldMatrix()
		{
			if (m_RecalculateTransform)
			{
				RecalculateTransform();
			}
			return m_WorldMatrix;
		}

		[[nodiscard]] const Vector3& GetWorldScale()
		{
			if (m_RecalculateTransform)
			{
				RecalculateTransform();
			}
			return m_WorldScale;
		}

		void Translate(const Vector3& pos)
		{
			m_WorldPosition = pos;
			m_RecalculateTransform = true;
		}

		void Move(const Vector3& direction)
		{
			m_WorldPosition += direction;
			m_RecalculateTransform = true;
		}

		void Rotate(float roll, float pitch, float yaw, bool isInDegrees)
		{
			const Vector3 rotation{ roll,pitch,yaw };
			Rotate(rotation, isInDegrees);
		}

		void Rotate(const Vector3& rotation, bool isInDegrees)
		{
			m_RecalculateTransform = true;
			if (isInDegrees)
			{
				m_LocalRotation = XMQuaternionRotationRollPitchYaw(-XMConvertToRadians(rotation.x), -XMConvertToRadians(rotation.y), -XMConvertToRadians(rotation.z));
			}
			else
			{
				m_LocalRotation = XMQuaternionRotationRollPitchYaw(-rotation.x, -rotation.y, -rotation.z);
			}
		}

		void Scale(const Vector3& scale)
		{
			m_LocalScale = scale;
			m_RecalculateTransform = true;
		}

		void Scale(float x, float y, float z)
		{
			const Vector3 scale{ x,y,z };
			Scale(scale);
		}
	};
}