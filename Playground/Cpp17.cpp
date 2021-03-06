#include "stdafx.h"


// 1. Variable templates
template <typename T> T pi = (T)3.141592;

// 3. Structure binding
std::tuple<int, float, char> x() { 
	return std::make_tuple<int, float, char>(5, 5.2f, 'c'); 
}

// 4. Optional parameter
std::optional<std::string> optionalFoo()
{
	if (1 == 1) {
		return "Hello";
	}
	return {};
}

// 5. Any
std::any anyFoo()
{
	std::string h("hello");
	return std::any(h);
}

// 6. Deprecated
void [[deprecated("This function is not safe")]] deprecatedFoo()
{
}

// 7. Variant as a error val (may be deprecated in favor of std::expected)
auto toInt(const std::string& s) -> std::variant<int, std::string>
{
	int i; 
	std::stringstream ss;
	ss << s; 
	ss >> i;

	if (ss.fail())
	{
		return s + " is not an intiger";
	}
	
	return i;
}


int Cpp17Samples()
{
	{ // 1. Variable templates
		double x = sin(2.0*440.0*pi<double>*0.01); // specializes pi as double
		float perimeter = 2.0f*pi<float>*0.01f; // specializes pi as float
	}

	{ // 3. Structure binding
		auto[a, b, c] = x();
		// The problem is that you can't use std::ignore, like in the old tie()
	}

	{ // 4. Optional parameter
		auto rv = optionalFoo();
		if (rv)
		{
			// We have a value, rv.value();
		}
		rv.value_or("dada"); // Either the value, or "dada"
	}

	{ // 5. Any
		try
		{
			auto r = std::any_cast<std::string>(anyFoo());
		}
		catch (...)
		{
		}
	}

	{ // 7. String view
		using namespace std::string_literals;
		auto a = "hello there"s;
		std::string_view largeStringView{ a.c_str(), a.size() };
	}

	{ // 7. String view
		if (auto i = toInt("1"); auto pi = std::get_if<int>(&i))
		{
			auto val = *pi;
		}
		else
		{
			auto err = std::get<std::string>(i);
		}
	}

    return 0;
}

namespace namespace1::namespace2::namespace3 {
	// 2. Namespace nesting
}
