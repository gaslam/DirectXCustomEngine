#pragma once

enum XboxControllerButton
{
	A,
	B,
	X,
	Y,
	DPADDOWN,
	DPADUP,
	DPADLEFT,
	DPADRIGHT,
	LSHOULDER,
	RSHOULDER,
	LSTICK,
	RSTICK,
	LSTICKDOWN,
	RSTICKDOWN,
	LSTICKUP,
	RSTICKUP,
	LSTICKLEFT,
	RSTICKLEFT,
	LSTICKRIGHT,
	RSTICKRIGHT,
	LTRIGGER,
	RTRIGGER
};

struct KeyInput {
	int id{};
	int controllerId{};
	XboxControllerButton controllerButton;
	DirectX::GamePad::ButtonStateTracker::ButtonState state;

	bool operator<(const KeyInput& rhs) const
	{
		return this->id < rhs.id;
	}
};