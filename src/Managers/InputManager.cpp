#include "pch.h"
#include "InputManager.h"

#include "Logger.h"
#include "KeyStructs/KeyStructs.h"
#include "Command/Command.h"

using namespace Engine;

InputManager::InputManager()
{
	m_pGamepad = std::make_unique<GamePad>();
	m_pMouse = std::make_unique<Mouse>();
	m_pKeyboard = std::make_unique<Keyboard>();

	const HWND hWnd{ GetActiveWindow() };
	m_pMouse->SetWindow(hWnd);
}

void InputManager::ProcessControllerInput(float /*deltaTime*/)
{
	for (int i{}; i < 4; ++i)
	{
		auto pad{ m_pGamepad->GetState(i) };
		if (!pad.IsConnected())
		{
			continue;
		}

		for (auto& command : m_Commands)
		{
			const auto controllerKey{ GetButtonState(command.first.controllerButton) };
			const int controllerId{ command.first.controllerId };
			if (controllerKey == command.first.state && i == controllerId)
			{
				command.second->Execute();
			}
		}
	}
}

void InputManager::BindButtonsToCommand(unsigned int id, XboxControllerButton& button, GamePad::ButtonStateTracker::ButtonState& state, Command* command)
{
	KeyInput input{};
	input.id = static_cast<int>(m_Commands.size());
	input.controllerId = id;
	input.controllerButton = button;
	input.state = state;
	m_Commands.insert({ input,std::unique_ptr<Command>(command) });
}

void InputManager::BindButtonsToInput(unsigned int id, XboxControllerButton& button,Keyboard::Keys& keyboardKey, const std::wstring& input)
{
	KeyInput keyInput{};
	keyInput.id = static_cast<int>(m_Commands.size());
	keyInput.controllerId = id;
	keyInput.controllerButton = button;
	keyInput.keyboardKey = keyboardKey;
	if(m_Inputs.contains(input))
	{
		const auto pLogger{ Logger::GetInstance() };
		pLogger->LogError(L"Input already exists. Try another value!!");
	}
	m_Inputs.insert({ input,keyInput });
}

void InputManager::Update()
{
	for (int i{}; i < 4; ++i)
	{
		auto pad{ m_pGamepad->GetState(i) };
		if (!pad.IsConnected())
		{
			continue;
		}
		m_buttons.Update(pad);
	}
	const auto pKeyState{ m_pKeyboard->GetState() };
	m_keyboardState.Update(pKeyState);
}

GamePad::ButtonStateTracker::ButtonState InputManager::GetButtonState(const XboxControllerButton& button) const
{
	switch (button)
	{
	case A:
		return m_buttons.a;
	case B:
		return m_buttons.b;
	case X:
		return m_buttons.x;
	case Y:
		return m_buttons.y;
	case DPADDOWN:
		return m_buttons.dpadDown;
	case DPADLEFT:
		return m_buttons.dpadLeft;
	case DPADRIGHT:
		return m_buttons.dpadRight;
	case DPADUP:
		return m_buttons.dpadUp;
	case LSHOULDER:
		return m_buttons.leftShoulder;
	case RSHOULDER:
		return m_buttons.rightShoulder;
	case LSTICK:
		return m_buttons.leftStick;
	case RSTICK:
		return m_buttons.rightStick;
	case LSTICKDOWN:
		return m_buttons.leftStickDown;
	case RSTICKDOWN:
		return m_buttons.rightStickDown;
	case LSTICKUP:
		return m_buttons.leftStickUp;
	case RSTICKUP:
		return m_buttons.rightStickUp;
	case LSTICKLEFT:
		return m_buttons.leftStickLeft;
	case RSTICKLEFT:
		return m_buttons.rightStickLeft;
	case LSTICKRIGHT:
		return m_buttons.leftStickRight;
	case RSTICKRIGHT:
		return m_buttons.rightStickRight;
	case LTRIGGER:
		return m_buttons.leftTrigger;
	case RTRIGGER:
		return m_buttons.rightTrigger;
	default: return {};
	}
}

bool InputManager::IsPressed(const std::wstring& input)
{
	const auto it{ std::find_if(m_Inputs.begin(), m_Inputs.end(), [input](const std::pair<std::wstring, KeyInput>& pair)
		{
			if (pair.first == input)
			{
				return true;
			}
			return false;
		}) };

	if (it == m_Inputs.end())
	{
		return false;
	}

	auto keyInput{ *it };
	const auto button{ GetButtonState(it->second.controllerButton) };
	if(button == m_buttons.PRESSED)
	{
		return true;
	}

	return m_keyboardState.IsKeyPressed(keyInput.second.keyboardKey);

}

bool InputManager::IsUp(const std::wstring& input)
{
	const auto it{ std::find_if(m_Inputs.begin(), m_Inputs.end(), [input](const std::pair<std::wstring, KeyInput>& pair)
		{
			if (pair.first == input)
			{
				return true;
			}
			return false;
		}) };

	if (it == m_Inputs.end())
	{
		return false;
	}

	const auto keyInput{ *it };
	const auto button{ GetButtonState(it->second.controllerButton) };

	if (button != m_buttons.UP || (GetAsyncKeyState(it->second.keyboardKey) & 0x8000) != 0)
	{
		return false;
	}

	return true;
}

bool InputManager::IsReleased(const std::wstring& input)
{
	const auto it{ std::find_if(m_Inputs.begin(), m_Inputs.end(), [input](const std::pair<std::wstring, KeyInput>& pair)
		{
			if (pair.first == input)
			{
				return true;
			}
			return false;
		}) };

	if (it == m_Inputs.end())
	{
		return false;
	}

	const auto keyInput{ *it };
	const auto button{ GetButtonState(it->second.controllerButton) };
	if(button == m_buttons.RELEASED)
	{
		return true;
	}

	return m_keyboardState.IsKeyReleased(keyInput.second.keyboardKey);
}

bool InputManager::IsHeld(const std::wstring& input)
{
	const auto it{ std::find_if(m_Inputs.begin(), m_Inputs.end(), [input](const std::pair<std::wstring, KeyInput>& pair)
		{
			if (pair.first == input)
			{
				return true;
			}
			return false;
		}) };

	if (it == m_Inputs.end())
	{
		return false;
	}

	auto keyInput{ *it };
	const auto button{ GetButtonState(it->second.controllerButton) };
	if (button == m_buttons.HELD || (GetAsyncKeyState(it->second.keyboardKey) & 0x8000) != 0)
	{
		return true;
	}
	return false;
}

void InputManager::Resume() const
{
	m_pGamepad->Resume();
}

void InputManager::Suspend() const
{
	m_pGamepad->Suspend();
}
