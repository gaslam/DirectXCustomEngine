#pragma once
using namespace DirectX;


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
	GamePad::ButtonStateTracker::ButtonState state;
	Keyboard::Keys keyboardKey;

	bool operator<(const KeyInput& rhs) const
	{
		return this->id < rhs.id;
	}
};