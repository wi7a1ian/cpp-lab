#include "stdafx.h"
#include "Crtp.h"

namespace CRPTest
{
	namespace ForStaticInterfacesWherebaseCanBeUsed
	{
		void Test() 
		{
			Constant42 c;
			printUsingInterface(c);
			printUsingImplementation(c);
		}
	}

	namespace BaseForCRTP
	{
		void Test() {
			Sensitivity s;
			s.setValue(2);
			s.scale(2);
			s.square();

			Scale<Sensitivity>& sc = s;
			sc.scale(2);

			Scale<decltype(s)>& sc2 = s;
			sc2.scale(2);
		}
	}

	namespace VariadicCRTP
	{
		void Test() {
			Solution1::X1234 x1;
			x1.extraMethod1();
			x1.extraMethod4();

			Solution2::X1234 x2;
			x2.extraMethod1();
			x2.extraMethod4();

			Solution3::X1234 x3;
			x3.extraMethod1();
			x3.extraMethod4();
		}
	}
}