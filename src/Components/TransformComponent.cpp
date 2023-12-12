#include "pch.h"
#include "TransformComponent.h"

#include "SceneClasses/GameObject.h"

Vector3 TransformComponent::GetWorldPosition()
{
	UpdateTransform();
	return m_WorldPosition;
}

Vector3 TransformComponent::GetForward()
{
	if(m_TransformChanged & ChangedTransform::Rotate)
	{
		UpdateTransform();
	}
	return m_Forward;
}

Vector4 TransformComponent::GetWorldRotation()
{

		UpdateTransform();
	return m_WorldRotation;
}


Vector3 TransformComponent::GetBackwards()
{

		UpdateTransform();
	return -m_Forward;
}


void TransformComponent::Translate(const Vector3& translation)
{
	m_LocalPosition =  translation;
	m_TransformChanged |= ChangedTransform::Translate;
}

void TransformComponent::Translate(float x, float y, float z)
{
	const Vector3 position{ x,y,z };
	Translate(position);
}

void TransformComponent::Rotate(float roll, float pitch, float yaw, bool isInDegrees)
{
	const Vector3 rotation{ roll,pitch,yaw };
	Rotate(rotation,isInDegrees);
}

void TransformComponent::Rotate(const Vector3& rotation, bool isInDegrees)
{
	m_TransformChanged |= ChangedTransform::Rotate;

	if(isInDegrees)
	{
		m_LocalRotation = XMQuaternionRotationRollPitchYaw( -XMConvertToRadians(rotation.x),-XMConvertToRadians(rotation.y),-XMConvertToRadians(rotation.z));
	}
	else
	{
		m_LocalRotation = XMQuaternionRotationRollPitchYaw(-rotation.x, -rotation.y, -rotation.z);
	}
}

void TransformComponent::SetScale(float scale)
{
	m_TransformChanged |= ChangedTransform::Scale;
	m_LocalScale = scale;
}

Vector3 TransformComponent::GetRight()
{
	UpdateTransform();
	return m_Right;
}
Vector3 TransformComponent::GetLeft()
{
	UpdateTransform();
	return -m_Right;
}



void TransformComponent::UpdateTransform()
{
	if(m_TransformChanged == ChangedTransform::None)
	{
		return;
	}
	const std::wstring warningMessage{ L"Cannot set world position on transform" };
	const auto owner{ GetOwner() };
	if (owner == nullptr)
	{
		Logger::LogWarning(warningMessage);
		return;
	}

	if(m_TransformChanged & ChangedTransform::Translate)
	{
		UpdateWorldPosition(owner);
	}

	if (m_TransformChanged & ChangedTransform::Rotate)
	{
		UpdateWorldRotation(owner);
	}

	if (m_TransformChanged & ChangedTransform::Scale)
	{
		UpdateWorldScale(owner);
	}

	m_TransformChanged = ChangedTransform::None;
}

void TransformComponent::UpdateWorldRotation(const GameObject* owner)
{

	const GameObject* parent{ owner->GetParent() };

	if(parent == nullptr)
	{
		m_WorldRotation = m_LocalRotation;
	}
	else
	{
		if (const auto transform{ owner->GetTransform() })
		{
			m_WorldRotation = m_LocalRotation + transform->GetWorldRotation();
		}
	}
	
	const auto rotMat = XMMatrixRotationQuaternion(m_WorldRotation);
	m_Forward = XMVector3TransformCoord(XMVectorSet(0.f,0.f,1.f,0.f), rotMat);
	m_Right = XMVector3TransformCoord(XMVectorSet(-1.f,0.f,0.f,0.f), rotMat);
	m_Up = XMVector3Cross(m_Forward, m_Right);
}

void TransformComponent::UpdateWorldPosition(const GameObject* owner)
{

	const auto parent{ owner->GetParent() };

	if(parent == nullptr)
	{
		m_WorldPosition = m_LocalPosition;
	}
	else
	{
		if (const auto transform{ owner->GetTransform() })
		{
			m_WorldPosition = transform->GetWorldPosition() + m_LocalPosition;
		}
	}
}

void TransformComponent::UpdateWorldScale(const GameObject* owner)
{

	const auto parent{ owner->GetParent() };

	if (parent == nullptr)
	{
		m_WorldScale = m_LocalScale;
	}
	else
	{
		if (const auto transform{ owner->GetTransform() })
		{
			m_WorldScale = transform->GetWorldScale() + m_WorldScale;
		}
	}
}