#pragma once

namespace DelegatingConstructors
{
	class class_c {
	public:
		int max;
		int min;
		int middle;

		class_c(int my_max) {
			max = my_max > 0 ? my_max : 10;
		}
		class_c(int my_max, int my_min) : class_c(my_max) {
			min = my_min > 0 && my_min < max ? my_min : 1;
		}
		class_c(int my_max, int my_min, int my_middle) : class_c(my_max, my_min) {
			middle = my_middle < max && my_middle > min ? my_middle : 5;
		}
	};

	/*class class_a {
	public:
		class_a() {}
		// member initialization here, no delegate
		class_a(string str) : m_string{ (string)str } {}

		//can’t do member initialization here
		// error C3511: a call to a delegating constructor shall be the only member-initializer
		class_a(string str, double dbl) : class_a(str), m_double{ dbl } {}

		// only member assignment
		class_a(string str, double dbl) : class_a(str) { m_double = dbl; }
		double m_double{ 1.0 };
		string m_string;
	};*/

	void test() {

		class_c c1{ 1, 3, 2 };
	}
}