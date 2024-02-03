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
		if(value.empty())
		{
			return {};
		}
		const int valueSize{ static_cast<int>(value.size()) };
		const int neededSize{ WideCharToMultiByte(CP_UTF8,0,value.data(),valueSize,nullptr,0,nullptr,nullptr) };
		std::string result(neededSize, 0);
		WideCharToMultiByte(CP_UTF8, 0, value.data(), valueSize, result.data(), neededSize, nullptr, nullptr);
		return result;
	}

	inline std::wstring EncodeUTF8(const std::string& value)
	{
		if(value.empty())
		{
			return {};
		}

		const int valueSize{static_cast<int>(value.size())};
		const int size_needed{ MultiByteToWideChar(CP_UTF8, 0, value.data(), valueSize, nullptr, 0) };
		std::wstring result(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, value.data(), valueSize, result.data(), size_needed);
		return result;
	}
}
