#pragma once
#include "pch.h"
#include "Main/Singleton.h"

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
}
