// ExportObjectExe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	auto obj = CreateObject();

	{
		std::vector<wchar_t> str{ 'd', 'u', 'p', 'a' };
		obj->SetCharVector(str);
		assert(str[0] == 'a');
	}

	{
		wchar_t str[] = L"dupa";
		obj->SetString(str);
		assert(str[0] == 'a');
	}

	{
		wchar_t* str = new wchar_t[5] {L"dupa"};
		obj->SetString(str);
		assert(str[0] == 'a');
		delete[] str;
	}

	{
		wchar_t str[] = L"dupa";
		obj->SetCharArray(str);
		assert(str[0] == 'a');
	}

	{
		auto safeStr = std::unique_ptr<wchar_t[]>(new wchar_t[5]{ L"dupa" });
		obj->ConsumeSafeString(std::move(safeStr));
	}

	{
		auto safeStr = std::unique_ptr<wchar_t[]>(new wchar_t[5]{ L"dupa" });
		obj->SetSafeString(safeStr);
		assert(safeStr[0] == 'a');
	}

	{
		std::shared_ptr<wchar_t> safeSharedStr = std::shared_ptr<wchar_t>(new wchar_t[5]{ L"dupa" }, std::default_delete<wchar_t[]>());
		obj->ShareSafeString(safeSharedStr);
		assert(safeSharedStr.get()[0] == 'a');
	}

    return 0;
}

