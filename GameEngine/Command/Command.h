#pragma once
#include "Main/Logger.h"

namespace Engine
{
	class Command
	{
	public:
		explicit Command() = default;
		virtual ~Command() = default;

		Command(const Command& other) = delete;
		Command(Command&& other) noexcept = delete;

		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) noexcept = delete;

		virtual void Execute() {}
	};

	class GameObject;
	class GameObjectCommand : public Command {
		GameObject* m_pOwner;
	protected:
		GameObject* GetCommandOwner() const { return m_pOwner; }
	public:
		explicit GameObjectCommand(GameObject* owner) : m_pOwner(owner) {}
		virtual ~GameObjectCommand() override = default;
		GameObjectCommand(const GameObjectCommand& other) = delete;
		GameObjectCommand(GameObjectCommand&& other) noexcept = delete;
		GameObjectCommand& operator=(const GameObjectCommand& other) = delete;
		GameObjectCommand& operator=(GameObjectCommand&& other) noexcept = delete;
	};

	class TestCommand: public Command
	{
	public:
		void Execute() override
		{
			const auto pLogger{ Logger::GetInstance() };
			pLogger->LogInfo(L"This is a test command");
		}
	};
}
