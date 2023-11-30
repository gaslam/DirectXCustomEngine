#include "pch.h"
#include "Logger.h"

#include <iostream>

extern void ExitGame() noexcept;

std::map<LogLevel, Logger::LogType> Logger::m_LogTypes{
	//Yellow
	{LogLevel::WARNING, LogType{ FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED,L"WARNING" }},
	//Red
	{ LogLevel::ERROR, LogType{ FOREGROUND_INTENSITY | FOREGROUND_RED,L"ERROR" } },
	//White
	{ LogLevel::INFO, LogType{ FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,L"INFO" } }
};

void Logger::Initialize()
{
#if defined(_DEBUG) || defined(DEBUG)
	if(AllocConsole())
	{
		FILE* pCout;
		freopen_s(&pCout, "CONIN$", "r", stdin);
		freopen_s(&pCout, "CONOUT$", "w", stdout);
		freopen_s(&pCout, "CONOUT$", "w", stderr);

		m_pConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		//Disable Close-Button
		HWND hwnd{ GetConsoleWindow() };
		if (hwnd != nullptr)
		{
			HMENU hMenu = GetSystemMenu(hwnd, FALSE);
			if (hMenu != nullptr) DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
		}
	}
	m_Os = &std::wcout;
#endif
}

void Logger::LogWarning(const InfoLog& info)
{
	HandleLog(LogLevel::WARNING, info);
}

void Logger::LogError(const InfoLog& info)
{
	HandleLog(LogLevel::ERROR, info);
}

void Logger::LogInfo(const InfoLog& info)
{
	HandleLog(LogLevel::INFO, info);
}



void Logger::HandleLog(LogLevel type, const InfoLog& info)
{
	CONSOLE_SCREEN_BUFFER_INFO   csbi;
	GetConsoleScreenBufferInfo(m_pConsoleHandle,&csbi);
	const WORD currentAttributeConsole{ csbi.wAttributes };

	const auto typeInfo{ m_LogTypes[type]};
	SYSTEMTIME st;
	GetSystemTime(&st);

	SetConsoleTextAttribute(m_pConsoleHandle, typeInfo.color);

	std::wcout << "[" << typeInfo.typeName << "]: " << info.warningText << "\n\n"
	<< "Date: " << st.wDay << "-" << st.wMonth << "-" << st.wYear << " (dd/mm/yyyy)" << "\n"
	<< "Time: " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << ":" << st.wMilliseconds << "\n"
	<< "File: " << info.fileLocation.file_name() <<  "(" << info.fileLocation.line()  << ")" << "\n"
	<< "Function: " << info.fileLocation.function_name() << "\n\n";

	SetConsoleTextAttribute(m_pConsoleHandle, currentAttributeConsole);

	if(type == LogLevel::ERROR)
	{
		MessageBoxW(0, info.warningText.c_str(), typeInfo.typeName.c_str(), MB_OK | MB_ICONERROR);
		ExitGame();
	}
}
