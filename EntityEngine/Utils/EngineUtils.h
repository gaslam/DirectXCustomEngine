#pragma once
#include <cmath>

namespace MathUtils
{
	inline bool IsEqual(const float n1,const float n2, const float epsilon = 0.1f)
	{
		const float result{ fabs(n1 - n2) };
		if(result < epsilon)
		{
			return true;
		}
		return false;
	}

	inline bool IsNotEqual(const float n1, const float n2, const float epsilon = 0.1f)
	{
		const float result{ fabs(n1 - n2) };
		if (result >= epsilon)
		{
			return true;
		}
		return false;
	}
}

namespace StringUtils
{
	inline std::string DecodeUTF8(const std::wstring& value)
	{
		const wchar_t* valueStr{value.c_str()};
		return DecodeUTF8(valueStr);
	}

	inline std::string DecodeUTF8(const wchar_t* value)
	{
		const int valueLen{ static_cast<int>(wcslen(value)) };
		if(valueLen <= 0)
		{
			return {};
		}
		const int neededSize{ WideCharToMultiByte(CP_UTF8,0,value,valueLen,nullptr,0,nullptr,nullptr) };
		std::string result(neededSize, 0);
		WideCharToMultiByte(CP_UTF8, 0, value, valueLen, result.data(), neededSize, nullptr, nullptr);
		return result;
	}

	inline std::wstring EncodeUTF8(const std::string& value)
	{
		const char* valueStr{ value.c_str() };
		return EncodeUTF8(valueStr);
	}

	inline std::wstring EncodeUTF8(const char* value)
	{
		const int valueLen{ static_cast<int>(strlen(value)) };
		if (valueLen <= 0)
		{
			return L"";
		}
		const int size_needed{ MultiByteToWideChar(CP_UTF8, 0, value, valueLen, nullptr, 0) };
		wchar_t* result = new wchar_t[size_needed + 1]; // +1 for null terminator
		MultiByteToWideChar(CP_UTF8, 0, value, valueLen, result, size_needed);
		result[size_needed] = L'\0'; // null-terminate the string
		return result;
	}
}
