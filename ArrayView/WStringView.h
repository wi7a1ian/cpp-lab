#pragma once

#include "ArrayView.h"

struct WStringView : ArrayView<wchar_t>
{
	// Default constructor
	WStringView()
	{
	}

	// Constructor
	WStringView(const wchar_t *const pStr, const size_t length) :
		ArrayView(pStr, length)
	{
	}

	// Convenience constructor
	WStringView(const wchar_t *const pStr) :
		ArrayView(pStr, wcslen(pStr))
	{
	}
};