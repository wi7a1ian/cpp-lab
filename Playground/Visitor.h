#pragma once
#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace VisitorPattern
{
	// the variant to visit
	using var_t = std::variant<int, long, double, std::string>;

	template<class T> struct always_false : std::false_type {};

	template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
	template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

	auto Test1()
	{
		std::vector<var_t> vec = { 10, 15l, 1.5, "hello" };

		for (auto& v : vec) {
			// Type-matching visitor: a class with 3 overloaded operator()'s
			std::visit(overloaded{
				[](auto arg) { std::cout << arg << ' '; },
				[](double arg) { std::cout << std::fixed << arg << ' '; },
				[](const std::string & arg) { std::cout << std::quoted(arg) << ' '; },
				}, v);
		}
	}

	auto Test2()
	{
		std::vector<var_t> vec = { 10, 15l, 1.5, "hello" };

		for (auto& v : vec) {
			var_t w = std::visit([](auto && arg) -> var_t {return arg + arg; }, v);

			std::visit([](auto && arg) {
				using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, int>)
					std::cout << "int with value " << arg << '\n';
				else if constexpr (std::is_same_v<T, long>)
					std::cout << "long with value " << arg << '\n';
				else if constexpr (std::is_same_v<T, double>)
					std::cout << "double with value " << arg << '\n';
				else if constexpr (std::is_same_v<T, std::string>)
					std::cout << "std::string with value " << std::quoted(arg) << '\n';
				else
					static_assert(always_false<T>::value, "non-exhaustive visitor!");
				}, w);
		}
	}
}