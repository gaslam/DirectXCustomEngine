#pragma once
#include <map>
#include <memory>

#include "Command/Command.h"
#include "KeyStructs/KeyStructs.h"
#include "Misc/Singleton.h"

namespace Engine
{

	class InputManager final : public Singleton<InputManager>
	{
	public:
		explicit InputManager();
		void ProcessControllerInput(float deltaTime);
		void BindButtonsToCommand(unsigned int id, XboxControllerButton& button, DirectX::GamePad::ButtonStateTracker::ButtonState& state, Command* command);
		void Update();
	private:
		using CommandsMap = std::map<KeyInput, std::unique_ptr<Command>>;
		using InputMap = std::map<std::string, KeyInput>;

		std::vector<int> m_ControllerIds{};
		DirectX::GamePad::ButtonStateTracker m_buttons;
		std::unique_ptr<DirectX::GamePad> m_pGamepad{};

		CommandsMap m_Commands{};
		InputMap m_Inputs{};
		DirectX::GamePad::ButtonStateTracker::ButtonState GetButtonState(const XboxControllerButton& button) const;
	};
}


