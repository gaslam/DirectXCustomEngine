#pragma once
#include "Shape.h"

class Cube : public Shape
{
public:
	Cube(float width = 1.f, float height = 1.f, float depth = 1.f, const Color& color = Color(Colors::White)) :
		m_Dimensions{ width,height,depth },
		m_Color{ color }
	{
	}

	Cube(const Vector3& dimensions, const Color& color = Color(Colors::White)) :
		m_Dimensions{ dimensions },
		m_Color{ color }
	{
	}

	Cube(const Vector3& dimensions, const std::wstring& textureLocation = L"", const Color& color = Color(Colors::White)) : Shape{ textureLocation },
		m_Dimensions{ dimensions },
		m_Color{ color }
	{
	}

	Cube(float width = 1.f, float height = 1.f, float depth = 1.f, const std::wstring& textureLocation = L"", const Color& color = Color(Colors::White)) : Shape{ textureLocation },
		m_Dimensions{ width,height,depth },
		m_Color{ color }
	{
	}
	virtual ~Cube() override = default;

	void SetDimensions(Vector3& dimensions);
	void SetDimensions(float width, float height, float depth);
	Vector3 GetDimensions() const { return m_Dimensions; }

	float GetWidth() const { return m_Dimensions.x; }
	float GetHeight() const { return m_Dimensions.y; }
	float GetDepth() const { return m_Dimensions.z; }
	Vector3 GetScale() const;

	void SetScale(Vector3& scale);
	void SetScale(float width, float height, float depth);
protected:
	void Initialize(const SceneContext& context) override;
private:
	Vector3 m_Dimensions{};
	Color m_Color{};
};

