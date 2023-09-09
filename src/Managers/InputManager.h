#pragma once
#include <map>
#include <memory>

#include "Command/Command.h"
#include "Structs/KeyStructs.h"
#include "Misc/Singleton.h"

using namespace DirectX;
using namespace std;

namespace Engine
{

	class InputManager final : public Singleton<InputManager>
	{
	public:
		explicit InputManager();
		void ProcessControllerInput(float deltaTime);
		void BindButtonsToCommand(unsigned int id, XboxControllerButton& button, GamePad::ButtonStateTracker::ButtonState& state, Command* command);
		void BindButtonsToInput(unsigned int id, XboxControllerButton& button, Keyboard::Keys& keyboardKey, const wstring& input);
		void Update();
		[[nodiscard]] GamePad* GetGamePad() const { return m_pGamepad.get(); }
		bool IsPressed(const wstring& input);
		bool IsUp(const wstring& input);
		bool IsReleased(const wstring& input);
		bool IsHeld(const wstring& input);
		void Resume() const;
		void Suspend() const;

		[[nodiscard]] Mouse::State GetMouseState() const { return m_pMouse->GetState(); }

	private:
		using CommandsMap = map<KeyInput, unique_ptr<Command>>;
		using InputMap = map<wstring, KeyInput>;

		vector<int> m_ControllerIds{};
		GamePad::ButtonStateTracker m_buttons;
		Keyboard::KeyboardStateTracker m_keyboardState;
		unique_ptr<GamePad> m_pGamepad{};
		unique_ptr<Keyboard> m_pKeyboard{};
		unique_ptr<Mouse> m_pMouse{};

		CommandsMap m_Commands{};
		InputMap m_Inputs{};
		GamePad::ButtonStateTracker::ButtonState GetButtonState(const XboxControllerButton& button) const;
	};
}


