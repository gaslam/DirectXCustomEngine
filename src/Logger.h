#pragma once
#include "Misc/Singleton.h"
#include <source_location>
#include <map>

using namespace std;

enum class LogLevel : UINT {
	WARNING,
	ERROR,
	INFO,
};

struct InfoLog
{
	wstring warningText{};
	source_location fileLocation{};
	InfoLog(const wchar_t* text, const source_location& location = source_location::current()) :
	warningText{text},
	fileLocation{location}
	{
		
	}
};

class Logger : public Singleton<Logger>
{
public:
	void Initialize();
	void LogWarning(const InfoLog& info) const;
	void LogError(const InfoLog& info) const;
	void LogInfo(const InfoLog& info) const;
private:
	HANDLE m_pConsoleHandle{};
	wostream* m_Os {nullptr};
	void HandleLog(LogLevel type, const InfoLog& info) const;
	struct LogType
	{
		WORD color;
		wstring typeName;
	};
	static map<LogLevel, LogType> m_LogTypes;
};

