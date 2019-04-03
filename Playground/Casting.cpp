#include "stdafx.h"
#include "SimpleInheritance.h"

namespace CastingTest{
	using namespace std;

	// http://msdn.microsoft.com/en-us/library/hh279667.aspx
	void CastingExample()
	{
		short x = 6;
		int y;

		y = (int)x; // old-style cast, old-style syntax
		y = int(x); // old-style cast, functional syntax

		// static_cast, for casts that are checked at compile time only. 
		// static_cast returns an error if the compiler detects that you are trying to cast between types that are completely incompatible. 
		// You can also use it to cast between pointer-to-base and pointer-to-derived, but the compiler can't always tell whether such conversions will be safe at runtime.
		y = static_cast<int>(x);       // No warning.


		// dynamic_cast, for safe, runtime-checked casts of pointer-to-base to pointer-to-derived. A dynamic_cast is safer than a static_cast for downcasts, but the runtime check incurs some overhead.
		Base* b = nullptr;
		auto d = new Derived(); // // Run-time check to determine whether b is actually a Derived*
		b = dynamic_cast<Base*>(d); // // Run-time check to determine whether b is actually a Derived*

		// If b was originally a Derived*, then d3 is a valid pointer.
		if (d)
		{
			// Safe to call Derived method.
			cout << d->DoNothing() << endl;
		}
		else
		{
			// Run-time check failed.
			cout << "d3 is null" << endl;
		}
	}
}