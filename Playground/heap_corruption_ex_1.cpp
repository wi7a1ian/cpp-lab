#include "stdafx.h"

namespace heap_corruption_ex_1
{
	void foo(const std::vector<int>& src)
	{
		std::vector<int> dest;

		//std::copy(src.begin(), src.end(), dest.begin()); // heap overflow - std::copy wont allocate memory for us!
		std::copy(src.begin(), src.end(), std::back_inserter(dest)); // fix

		std::cout << src.size() << std::endl;
		std::cout << dest.size() << std::endl;
	}

	int main()
	{
		std::vector<int> bar{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14 };
		foo(bar);

		return 0;
	}
}