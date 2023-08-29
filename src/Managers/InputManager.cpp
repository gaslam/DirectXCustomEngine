#include "pch.h"
#include "InputManager.h"

#include "Logger.h"
#include "KeyStructs/KeyStructs.h"
#include "Command/Command.h"

Engine::InputManager::InputManager()
{
	m_pGamepad = std::make_unique<DirectX::GamePad>();
}

void Engine::InputManager::ProcessControllerInput(float /*deltaTime*/)
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

void Engine::InputManager::BindButtonsToCommand(unsigned int id, XboxControllerButton& button, DirectX::GamePad::ButtonStateTracker::ButtonState& state, Engine::Command* command)
{
	KeyInput input{};
	input.id = static_cast<int>(m_Commands.size());
	input.controllerId = id;
	input.controllerButton = button;
	input.state = state;
	m_Commands.insert({ input,std::unique_ptr<Engine::Command>(command) });
}

void Engine::InputManager::Update()
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
}

DirectX::GamePad::ButtonStateTracker::ButtonState Engine::InputManager::GetButtonState(const XboxControllerButton& button) const
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
