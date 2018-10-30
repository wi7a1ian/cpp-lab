#pragma once
#include <algorithm>
#include <vector>

namespace generic_select
{
	template <
		typename T, typename A, typename F,
		typename R = typename std::decay<typename std::result_of<typename std::decay<F>::type&(typename std::vector<T, A>::const_reference)>::type>::type>
		std::vector<R> select(std::vector<T, A> const & c, F&& f)
	{
		std::vector<R> v;
		std::transform(
			std::cbegin(c), std::cend(c),
			std::back_inserter(v),
			std::forward<F>(f));

		return v;
	}


	struct shoe
	{
		int id;
		std::string name;
		uint64_t size;
	};

	int main()
	{
		std::vector<shoe> shoes{
		   {0, "Sneakers", 42},
		   {1, "Sandals", 44},
		   {2, "Converse", 41} };

		auto names = select(shoes, [](shoe const & b) { return b.name; });

		for (auto const& name : names)
		{
			auto a = name;
		}

		return 0;
	}
}