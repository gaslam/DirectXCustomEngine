#include "pch.h"
#include "TransformComponent.h"

#include "SceneClasses/GameObject.h"

Vector3 TransformComponent::GetWorldPosition()
{
	if (IsDirty())
	{
		UpdateTransform();
	}
	return m_WorldPosition;
}

Vector3 TransformComponent::GetForward()
{
	if(IsDirty())
	{
		UpdateTransform();
	}

	return m_Forward;
}

Vector4 TransformComponent::GetWorldRotation()
{
	if (IsDirty())
	{
		UpdateTransform();
	}
	return m_WorldRotation;
}


Vector3 TransformComponent::GetBackwards()
{
	if (IsDirty())
	{
		UpdateTransform();
	}
	return -m_Forward;
}


void TransformComponent::Translate(const Vector3& translation)
{
	m_LocalPosition = translation;
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
	Rotate(rotation, isInDegrees);
}

void TransformComponent::Rotate(const Vector3& rotation, bool isInDegrees)
{
	m_TransformChanged |= ChangedTransform::Rotate;

	if (isInDegrees)
	{
		m_LocalRotation = XMQuaternionRotationRollPitchYaw(-XMConvertToRadians(rotation.x), -XMConvertToRadians(rotation.y), -XMConvertToRadians(rotation.z));
	}
	else
	{
		m_LocalRotation = XMQuaternionRotationRollPitchYaw(-rotation.x, -rotation.y, -rotation.z);
	}
}

void TransformComponent::Update()
{
	const auto owner{ GetOwner() };
	const auto parent{ owner->GetParent() };
	if (parent == nullptr)
	{
		return;
	}
	const auto pTransform{ parent->GetTransform() };
	if (pTransform->IsDirty())
	{
		m_TransformChanged |= ChangedTransform::Rotate;
		m_TransformChanged |= ChangedTransform::Translate;
		UpdateTransform();
	}
}

void TransformComponent::SetScale(Vector3 scale)
{
	m_TransformChanged |= ChangedTransform::Scale;
	m_LocalScale = scale;
}

void TransformComponent::SetScale(float value)
{
	SetScale({ value,value,value });
}

Vector3 TransformComponent::GetWorldScale()
{
	UpdateTransform();
	return  m_WorldScale;
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
	if (!IsDirty())
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

	if (m_TransformChanged & ChangedTransform::Translate)
	{
		UpdateWorldPosition(owner);
	}

	if (m_TransformChanged & ChangedTransform::Rotate)
	{
		UpdateWorldRotation(owner);
	}

	if (m_TransformChanged & ChangedTransform::Scale)
	{
		UpdateWorldScale();
	}


	m_TransformChanged = ChangedTransform::None;
}

void TransformComponent::UpdateWorldRotation(const GameObject* owner)
{

	const GameObject* parent{ owner->GetParent() };

	if (parent == nullptr)
	{
		m_WorldRotation = m_LocalRotation;
	}
	else
	{
		if (const auto transform{ parent->GetTransform() })
		{
			m_WorldRotation = transform->GetWorldRotation() * m_LocalRotation;
		}
	}

	const auto rotMat = XMMatrixRotationQuaternion(m_WorldRotation);
	m_Forward = XMVector3TransformCoord(XMVectorSet(0.f, 0.f, 1.f, 0.f), rotMat);
	m_Right = XMVector3TransformCoord(XMVectorSet(-1.f, 0.f, 0.f, 0.f), rotMat);
	m_Up = XMVector3Cross(m_Forward, m_Right);
	ChangeWorldMatrix(owner);
}

void TransformComponent::ChangeWorldMatrix(const GameObject* owner)
{
	m_World = XMMatrixScaling(m_WorldScale.x, m_WorldScale.y, m_WorldScale.z) * XMMatrixRotationQuaternion(m_WorldRotation) * XMMatrixTranslation(m_WorldPosition.x, m_WorldPosition.y, m_WorldPosition.z);

	if (const GameObject * parent{ owner->GetParent() })
	{
		if (const auto pTransform{ parent->GetTransform() })
			m_World *= pTransform->GetWorldMatrix();
	}
}

void TransformComponent::UpdateWorldPosition(const GameObject* owner)
{

	const auto parent{ owner->GetParent() };

	if (parent == nullptr)
	{
		m_WorldPosition = m_LocalPosition;
	}
	else
	{
		if (const auto transform{ parent->GetTransform() })
		{
			m_WorldPosition = transform->GetWorldPosition() + m_LocalPosition;
		}
	}
	ChangeWorldMatrix(owner);
}

void TransformComponent::UpdateWorldScale()
{

	m_WorldScale = m_LocalScale;

	const GameObject* owner{ GetOwner() };

	ChangeWorldMatrix(owner);
}