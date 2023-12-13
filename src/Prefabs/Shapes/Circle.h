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

	virtual ~Circle() override = default;

	void SetDiameter(float diameter);
	float GetRadius() const { return m_Diameter / 2.f; }
	float GetDiameter() const { return m_Diameter; }

protected:
	void Initialize(const SceneContext& context) override;
private:
	float m_Diameter{ 5.f };
	Color m_Color{};
};

