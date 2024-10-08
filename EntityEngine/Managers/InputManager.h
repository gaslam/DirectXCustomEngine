#pragma once
#include "pch.h"
#include <map>
#include <memory>

#include "Command/Command.h"
#include "Main/Singleton.h"
#include "Structs/KeyStructs.h"
#include "Mouse.h"

using namespace DirectX;
using namespace std;

namespace EntityEngine
{

	class InputManager final : public Singleton<InputManager>
	{
	public:
		explicit InputManager();
		void ProcessControllerInput(float deltaTime);
		void BindButtonsToCommand(const unsigned int id, const XboxControllerButton& button, const Keyboard::Keys& key, const GamePad::ButtonStateTracker::ButtonState& state, Command* command);
		void BindButtonsToInput(unsigned int id, const std::wstring& input, Keyboard::Keys& keyboardKey, XboxControllerButton button = NONE);
		void Update();
		[[nodiscard]] SimpleMath::Vector2 GetJoystickCoords(XboxJoystick joystick, unsigned int controllerId);
		[[nodiscard]] GamePad* GetGamePad() const { return m_pGamepad.get(); }
		bool IsPressed(const wstring& input);
		bool IsUp(const wstring& input);
		bool IsReleased(const wstring& input);
		bool IsHeld(const wstring& input);
		void Resume() const;
		void Suspend() const;
		void SetMouseMode(const Mouse::Mode& mode, const bool isVisible = false) const
		{
			m_pMouse->SetMode(mode);
			m_pMouse->SetVisible(isVisible);
		}

		[[nodiscard]] Mouse::State GetMouseState() const { return m_pMouseHandler->GetState(); }
		[[nodiscard]] SimpleMath::Vector2 GetMouseDelta() const { return m_pMouseHandler->GetDelta(); }

	private:

		class MouseHandler final
		{
		public:
			explicit MouseHandler(Mouse* mouse) : m_pMouse(std::unique_ptr<Mouse>(mouse))
			{

			}

			void UpdateDelta()
			{
				const auto mouseState(m_pMouse->GetState());
				if (mouseState.positionMode == Mouse::MODE_RELATIVE)
				{
					m_CurrentPos = SimpleMath::Vector2{ static_cast<float>(mouseState.x),static_cast<float>(mouseState.y) };
					if (m_CurrentPos.LengthSquared() == 0.0f)
					{
						m_CurrentPos = m_PreviousPos;
					}
					m_Delta = m_CurrentPos - m_PreviousPos;

					m_CurrentPos = m_PreviousPos;
				}
			}

			void SetMode(const Mouse::Mode& mode) const
			{
				m_pMouse->SetMode(mode);
			}
			[[nodiscard]] Mouse::State GetState() const { return m_pMouse->GetState(); };
			[[nodiscard]] SimpleMath::Vector2 GetDelta() const { return m_Delta; };
		private:
			unique_ptr<Mouse> m_pMouse{};
			SimpleMath::Vector2 m_Delta{};
			SimpleMath::Vector2 m_PreviousPos{}, m_CurrentPos{};
		};

		using CommandsMap = map<KeyInput, unique_ptr<Command>>;
		using InputMap = map<wstring, KeyInput>;

		vector<int> m_ControllerIds{};
		GamePad::ButtonStateTracker m_buttons;
		Keyboard::KeyboardStateTracker m_keyboardState;
		unique_ptr<GamePad> m_pGamepad{};
		unique_ptr<Keyboard> m_pKeyboard{};
		unique_ptr<MouseHandler> m_pMouseHandler{};
		Mouse* m_pMouse{ nullptr };

		CommandsMap m_Commands{};
		InputMap m_Inputs{};
		GamePad::ButtonStateTracker::ButtonState GetButtonState(const XboxControllerButton& button) const;
	};
}


