#pragma once
#include "BaseComponent.h"
#include <type_traits>

using namespace Engine;
using namespace DirectX;
using namespace SimpleMath;

enum class ChangedTransform
{
	Translate = 1 << 0,
	Rotate = 1 << 1,
	Scale = 1 << 2,
	None = 1 << 3,
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
	void Update(const SceneContext&) override;
	void SetScale(Vector3 scale = {1.f,1.f,1.f});
	[[nodiscard]] Vector3 GetWorldScale();

	[[nodiscard]]Vector3 GetWorldPosition();
	[[nodiscard]]Vector3 GetForward();
	[[nodiscard]]Vector4 GetWorldRotation();
	[[nodiscard]]Vector3 GetBackwards();
	[[nodiscard]]Vector3 GetRight();
	[[nodiscard]]Vector3 GetLeft();
	[[nodiscard]] Matrix GetWorldMatrix() const { return m_World; }
	[[nodiscard]] bool IsDirty() const
	{
		const bool isTranslateDirty{ m_TransformChanged & ChangedTransform::Translate };
		const bool isRotateDirty{ m_TransformChanged & ChangedTransform::Rotate };
		const bool isScaleDirty{ m_TransformChanged & ChangedTransform::Scale };

		return isTranslateDirty || isRotateDirty || isScaleDirty;
	}
private:

	Vector3 m_WorldPosition{};
	Vector3 m_LocalPosition{};
	Vector3 m_WorldScale{1.f,1.f,1.f};
	Vector3 m_LocalScale{m_WorldScale};
	Vector3 m_Forward{Vector3::Forward};
	Vector3 m_Right{Vector3::Right};
	Vector3 m_Up{Vector3::Up};
	Vector4 m_WorldRotation{};
	Vector4 m_LocalRotation{ m_WorldRotation };

	Matrix m_World{Matrix::Identity};

	ChangedTransform m_TransformChanged{};


	void UpdateTransform();
	void UpdateWorldPosition(const GameObject* owner);
	void UpdateWorldScale(const GameObject* owner);
	void UpdateWorldRotation(const GameObject* owner);
	void ChangeWorldMatrix();
};