#pragma once
#include "Shape.h"
class Circle :
	public Shape
{
public:
	Circle(float diameter, const Color& color = Color(Colors::White)) :
	m_Diameter{diameter},
		m_Color{ color }
	{
	}

	Circle(float diameter,const std::wstring& textureLocation ,  const Color& color = Color(Colors::White)) : Shape(textureLocation),
		m_Diameter{ diameter },
		m_Color{ color }
	{
	}

	virtual ~Circle() override = default;

	void RenderImGui() override;

	void SetDiameter(float diameter);
	float GetRadius() const { return m_Diameter / 2.f; }
	float GetDiameter() const { return m_Diameter; }

protected:
	void Initialize() override;
	void Update() override;
private:
	void ResizeSphere();
	float m_Diameter{ 5.f };
	Color m_Color{};
	std::unique_ptr<GeometricPrimitive> m_pSphere{};
	bool m_IsMarkedForResize{ true };
};

