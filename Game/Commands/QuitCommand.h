#pragma once
#include "Command/Command.h"

extern void ExitGame() noexcept;
class QuitCommand : public Engine::Command {
public:
	explicit QuitCommand() = default;

	//TODO: Fix warnings that are caused by the default constructor being deleted in base class. For now this works
	~QuitCommand() override = default;

	QuitCommand(const QuitCommand&) = delete;
	QuitCommand(QuitCommand&&) noexcept = delete;
	QuitCommand& operator=(const QuitCommand&) = delete;
	QuitCommand& operator=(QuitCommand&&) noexcept = delete;
	void Execute() override {
		ExitGame();
	}
};