#pragma once
#include <iostream>
#include <string>

namespace StringTypedef
{
	template<typename T, typename tag>
	class strong_typedef : public T {};

	using filename_t = strong_typedef<std::string, class filename_t_tag>;
	using url_t = strong_typedef<std::string, class filename_t_tag>;

	auto read(filename_t filename) { return "foo"; }
	auto read(url_t url) { return "bar"; }

	auto Test()
	{
		auto filename = filename_t{ "foobar.txt" };
		auto url = url_t{ "jttp://foobar.com" };

		std::cout << "From disk [" << filename << "]: " << read(filename) << std::endl;
		std::cout << "From web [" << url << "]: " << read(url) << std::endl;
	}
}