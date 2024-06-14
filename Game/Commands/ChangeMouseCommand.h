#pragma once
#include "Managers/InputManager.h"

using namespace Engine;
class ChangeMouseCommand : public Command {
public:
	void Execute() override {
		const auto input{ InputManager::GetInstance() };
		if (input)
		{
			const Mouse::State mouseState{ input->GetMouseState() };
			const bool isRelative{ mouseState.positionMode == Mouse::MODE_RELATIVE };
			const Mouse::Mode mouseMode{ isRelative ? Mouse::MODE_ABSOLUTE : Mouse::MODE_RELATIVE };
			input->SetMouseMode(mouseMode,!isRelative);
		}
	}
};