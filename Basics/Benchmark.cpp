#include "stdafx.h"

namespace UsingChronoLibrary {

	void test2()
	{

		auto n = 1000000;
		std::vector<int> vector(n, 1);

		auto t1 = std::chrono::steady_clock::now();
		auto sum = std::accumulate(vector.begin(), vector.end(), 0);
		auto t2 = std::chrono::steady_clock::now();

		using nano_s = std::chrono::nanoseconds;
		using micro_s = std::chrono::microseconds;
		using milli_s = std::chrono::milliseconds;
		using seconds = std::chrono::seconds;
		using minutes = std::chrono::minutes;
		using hours = std::chrono::hours;

		auto d_nano = std::chrono::duration_cast<nano_s>(t2 - t1).count();
		auto d_micro = std::chrono::duration_cast<micro_s>(t2 - t1).count();
		auto d_milli = std::chrono::duration_cast<milli_s>(t2 - t1).count();
		auto d_s = std::chrono::duration_cast<seconds>(t2 - t1).count();
		auto d_m = std::chrono::duration_cast<minutes>(t2 - t1).count();
		auto d_h = std::chrono::duration_cast<hours>(t2 - t1).count();

		std::cout << "sum:      " << sum << "\n"
			<< "d_nano:   " << d_nano << "\n"
			<< "d_micro:  " << d_micro << "\n"
			<< "d_milli:  " << d_milli << "\n"
			<< "d_s:      " << d_s << "\n"
			<< "d_m:      " << d_m << "\n"
			<< "d_h:      " << d_h << "\n"
			<< std::endl;


		for (auto numEl = n; numEl != 11 * n; numEl += n)
		{
			Benchmark<> benchmark;
			std::vector<int> vector(numEl, 1);

			using Iterator = decltype(vector.begin());
			auto results = benchmark.benchmark(std::accumulate< Iterator, int>,
				vector.begin(), vector.end(), 0);


			std::cout << "#elements: " << std::setw(9) << numEl << " "
				<< " mean: " << std::setw(8) << benchmark.mean()
				<< " st. dev: : " << std::setw(8)
				<< benchmark.standard_deviation()
				<< std::endl;

		}
	}
}