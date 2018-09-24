#pragma once
#ifndef DLLCALL_H
#define DLLCALL_H

// C system files.
// None

// C++ system files.
#include <windows.h>
#include <tuple>
#include <functional>

// Other libraries' .h files.
// None

// Our project's .h files.
// None


// Declaration and implementation
namespace DLLCALL
{
	using namespace std;

	// Pack of numbers.
	// Nice idea, found at http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
	template<int ...> struct seq {};

	// Metaprogramming Expansion
	template<int N, int ...S> struct gens : gens < N - 1, N - 1, S... > {};
	template<int ...S> struct gens < 0, S... >
	{
		typedef seq<S...> type;
	};

	// Function that performs the actual call
	template<typename R, int ...S, typename...Args>
	R ActualCall(seq<S...>, std::tuple<Args...> tpl, std::function<R(Args...)> func)
	{
		// It calls the function while expanding the std::tuple to it's arguments via std::get<S>
		return func(std::get<S>(tpl) ...);
	}

#pragma warning(disable:4290)

	// Functions to be called from your code
	template <typename R, typename...Args> R DLLCall(const wchar_t* module, const char* procname, std::tuple<Args...> tpl) throw(HRESULT)
	{
		HMODULE hModule = LoadLibrary(module);
		FARPROC fp = GetProcAddress(hModule, procname);
		if (!fp)
			throw E_POINTER;
		typedef R(__stdcall *function_pointer)(Args...);
		function_pointer P = (function_pointer)fp;

		std::function<R(Args...)> f = P;
		R return_value = ActualCall(typename gens<sizeof...(Args)>::type(), tpl, f);
		FreeLibrary(hModule);
		return return_value;
	}

	// Parameterized GetProcAddress
	template <typename R, typename ...Args> std::function<R(Args...)> GetProcAddress(HMODULE hModule, const char* procname)
	{
		FARPROC fp = GetProcAddress(hModule, procname);
		if (!fp)
			return nullptr;
		typedef R(__stdcall *function_pointer)(Args...);
		function_pointer P = (function_pointer)fp;
		std::function<R(Args...)> f = P;
		return f;
	}
}

#endif