#pragma once
#include "Main/NullTimer.h"
#include "Main/Timer.h"
#include "Utils/GameHandler.h"

using namespace DX;

namespace Engine
{
	class Locator
	{
	public:
		static void Initialize()
		{
			m_pTimer = std::make_unique<NullTimer>();
			m_pGameHandler = std::make_unique<NullGameHandler>();
		}

		static Timer* GetTimer() { return m_pTimer.get(); }
		static GameHandlerBase* GetGameHandler() { return m_pGameHandler.get(); }

		static void Provide(std::unique_ptr<Timer>&& pTimer)
		{

			m_pTimer = pTimer != nullptr ? std::move(pTimer) : std::make_unique<NullTimer>();
		}

		static void Provide(std::unique_ptr<GameHandlerBase>&& pGameHandler)
		{

			m_pGameHandler = pGameHandler != nullptr ? std::move(pGameHandler) : std::make_unique<NullGameHandler>();
			m_pGameHandler->Initialize();
		}
	private:
		inline static std::unique_ptr<Timer> m_pTimer{};
		inline static std::unique_ptr<GameHandlerBase> m_pGameHandler{};
	};
}

