#pragma once
#include "Command/Command.h"

extern void ExitGame() noexcept;
class QuitCommand : public Engine::Command {
public:
	void Execute() override {
		ExitGame();
	}
};