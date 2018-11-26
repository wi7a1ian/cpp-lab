#pragma once

#include <vector>

// https://blog.therocode.net/2018/10/for-each-with-index
namespace enumerate_with_index
{
	template <typename container_type>
	struct enumerate_wrapper
	{
		using iterator_type = std::conditional_t<std::is_const_v<container_type>, typename container_type::const_iterator, typename container_type::iterator>;
		using pointer_type = std::conditional_t<std::is_const_v<container_type>, typename container_type::const_pointer, typename container_type::pointer>;
		using reference_type = std::conditional_t<std::is_const_v<container_type>, typename container_type::const_reference, typename container_type::reference>;
	
		struct enumerate_wrapper_iter
		{
			size_t index;
			iterator_type value;

			enumerate_wrapper_iter& operator++()
			{
				++index;
				++value;
				return *this;
			}

			bool operator!=(const iterator_type& other) const
			{
				return value != other;
			}

			std::pair<size_t, reference_type> operator*() {
				return std::pair<size_t, reference_type>{index, *value};
			}
		};


		constexpr enumerate_wrapper(container_type& c) : container(c)
		{
			// nop
		}

		constexpr enumerate_wrapper_iter begin()
		{
			//return with 0 becuase this is the first index of the looping
			return { 0, std::begin(container) };
		}

		constexpr iterator_type end()
		{
			return std::end(container);
		}

		container_type& container;
	};

	template <typename container_type>
	constexpr auto enumerate(container_type& c)
	{
		return enumerate_wrapper(c);
	}


	int main()
	{
		std::vector<int> ts{1, 2, 3, 4, 5, 6};

		for (auto[i, t] : enumerate(ts))
		{
			auto ii = i;
			auto tt = t;
			//can modify t
		}

		return 0;
	}

}