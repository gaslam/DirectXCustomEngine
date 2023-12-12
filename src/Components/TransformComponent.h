#pragma once
#include "BaseComponent.h"
#include <type_traits>

using namespace Engine;
using namespace DirectX;
using namespace SimpleMath;

enum class ChangedTransform
{
	None = 0x00,
	Translate = 0x01,
	Rotate = 0x02,
	Scale = 0x04,
};



inline ChangedTransform operator | (ChangedTransform lhs, ChangedTransform rhs)
{
	using T = std::underlying_type_t <ChangedTransform>;
	return static_cast<ChangedTransform>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

inline bool operator & (ChangedTransform lhs, ChangedTransform rhs)
{
	using T = std::underlying_type_t <ChangedTransform>;
	return static_cast<ChangedTransform>(static_cast<T>(lhs) | static_cast<T>(rhs)) == lhs;
}

inline ChangedTransform& operator |= (ChangedTransform& lhs, ChangedTransform rhs)
{
	lhs = lhs | rhs;
	return lhs;
}

class TransformComponent : public BaseComponent
{

public:
	void Initialize(const SceneContext&) override{};
	void Translate(const Vector3& translation);
	void Translate(float x = 0.f,float y = 0.f, float z = 0.f);
	void Rotate(float roll = 0.f, float pitch = 0.f, float yaw = 0.f, bool isInDegrees = false);
	void Rotate(const Vector3& rotation, bool isInDegrees = false);
	void SetScale(float scale = 1.f);
	float GetWorldScale() const { return m_WorldScale; }

	Vector3 GetWorldPosition();
	Vector3 GetForward();
	Vector4 GetWorldRotation();
	Vector3 GetBackwards();
	Vector3 GetRight();
	Vector3 GetLeft();
private:

	Vector3 m_WorldPosition{};
	Vector3 m_LocalPosition{};
	Vector3 m_Forward{Vector3::Forward};
	Vector3 m_Right{Vector3::Right};
	Vector3 m_Up{Vector3::Up};
	Vector4 m_WorldRotation{};
	Vector4 m_LocalRotation{ m_WorldRotation };

	ChangedTransform m_TransformChanged{};

	float m_WorldScale{};
	float m_LocalScale{};


	void UpdateTransform();
	void UpdateWorldPosition(const GameObject* owner);
	void UpdateWorldScale(const GameObject* owner);
	void UpdateWorldRotation(const GameObject* owner);
};