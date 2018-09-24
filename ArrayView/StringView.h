#pragma once

#include "ArrayView.h"

struct StringView : ArrayView<char>
{
	// Default constructor
	StringView()
	{
	}

	// Constructor
	StringView(const char *const pStr, const size_t length) :
		ArrayView(pStr, length)
	{
	}

	// Convenience constructor
	StringView(const char *const pStr) :
		ArrayView(pStr, strlen(pStr))
	{
	}
};