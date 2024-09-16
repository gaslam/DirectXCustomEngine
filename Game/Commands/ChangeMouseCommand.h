#pragma once
#include "Managers/InputManager.h"

using namespace Engine;
class ChangeMouseCommand : public Command {
public:

	explicit ChangeMouseCommand() = default;

	//TODO: Fix warnings that are caused by the default constructor being deleted in base class. For now this works
	~ChangeMouseCommand() override = default;

	ChangeMouseCommand(const ChangeMouseCommand&) = delete;
	ChangeMouseCommand(ChangeMouseCommand&&) noexcept = delete;
	ChangeMouseCommand& operator=(const ChangeMouseCommand&) = delete;
	ChangeMouseCommand& operator=(ChangeMouseCommand&&) noexcept = delete;
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