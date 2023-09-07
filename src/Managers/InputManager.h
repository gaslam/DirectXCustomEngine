#pragma once
#include <map>
#include <memory>

#include "Command/Command.h"
#include "KeyStructs/KeyStructs.h"
#include "Misc/Singleton.h"

using namespace DirectX;

namespace Engine
{

	class InputManager final : public Singleton<InputManager>
	{
	public:
		explicit InputManager();
		void ProcessControllerInput(float deltaTime);
		void BindButtonsToCommand(unsigned int id, XboxControllerButton& button, GamePad::ButtonStateTracker::ButtonState& state, Command* command);
		void BindButtonsToInput(unsigned int id, XboxControllerButton& button, Keyboard::Keys& keyboardKey, const std::wstring& input);
		void Update();
		[[nodiscard]] GamePad* GetGamePad() const { return m_pGamepad.get(); }
		bool IsPressed(const std::wstring& input);
		bool IsUp(const std::wstring& input);
		bool IsReleased(const std::wstring& input);
		bool IsHeld(const std::wstring& input);
		void Resume();
		void Suspend();

		[[nodiscard]] Mouse::State GetMouseState() const { return m_pMouse->GetState(); }

	private:
		using CommandsMap = std::map<KeyInput, std::unique_ptr<Command>>;
		using InputMap = std::map<std::wstring, KeyInput>;

		std::vector<int> m_ControllerIds{};
		GamePad::ButtonStateTracker m_buttons;
		Keyboard::KeyboardStateTracker m_keyboardState;
		std::unique_ptr<GamePad> m_pGamepad{};
		std::unique_ptr<Keyboard> m_pKeyboard{};
		std::unique_ptr<Mouse> m_pMouse{};

		CommandsMap m_Commands{};
		InputMap m_Inputs{};
		GamePad::ButtonStateTracker::ButtonState GetButtonState(const XboxControllerButton& button) const;
	};
}


