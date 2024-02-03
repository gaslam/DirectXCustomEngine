#pragma once
#include "Main/Singleton.h"
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
	InfoLog(const wstring text, const source_location& location = source_location::current()) :
		warningText{ text },
		fileLocation{ location }
	{

	}
};

class Logger : public Singleton<Logger>
{
public:
	void Initialize();
	static void LogWarning(const InfoLog& info);
	static void LogError(const InfoLog& info);
	static void LogInfo(const InfoLog& info);
private:
	static inline HANDLE m_pConsoleHandle{};
	static inline wostream* m_Os {nullptr};
	static void HandleLog(LogLevel type, const InfoLog& info);
	struct LogType
	{
		WORD color;
		wstring typeName;
	};
	static map<LogLevel, LogType> m_LogTypes;
};

