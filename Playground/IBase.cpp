#include "stdafx.h"
#include "IBase.h"

namespace BaseClassTest
{
	void DerivFromIBase::Free()
	{
	}

	void BaseClassTest::DerivFromBase::Free()
	{
	}

	void Parent::Something() {};
	void DerivFromParent::Something() {};


	void BaseClassExample1()
	{
		DerivFromIBase d1{};
		DerivFromBase d2{};
		DerivFromParent d3{};

		d1.SomeValue = 666;
		d2.SomeValue = 666;
		d3.SomeValue = 666;

		IBase& b1 = d1;
		IBase& b2 = d2;
		Base& b3 = d2;
		Parent& b4 = d3;

		//auto x1 = b1; // Copy abstract base not allowed
		//auto x2 = b2; // Copy abstract base not allowed
		//auto x3 = b3; // Copy abstract base not allowed
		Parent x4 = b4; // Copy base allowed
		//auto y4 = dynamic_cast<DerivFromParent&>(x4); // but crash
		auto* y4 = (DerivFromParent*)(&x4);
		assert(y4->SomeValue != d3.SomeValue); // Cast deriv allowed but sliced, we lost SomeValue

		DerivFromIBase y1 = d1; 
		DerivFromBase y2 = d2;
		assert(y1.SomeValue == y2.SomeValue); // Copy deriv allowed

		auto c1 = dynamic_cast<DerivFromIBase&>(b1);
		auto c2 = dynamic_cast<DerivFromBase&>(b2);
		auto c3 = dynamic_cast<DerivFromBase&>(b3);
		auto c4 = dynamic_cast<DerivFromParent&>(b4);
		assert(c1.SomeValue == c2.SomeValue); // Cast deriv allowed
		assert(c2.SomeValue == c3.SomeValue); // Cast deriv allowed
		assert(c3.SomeValue == c4.SomeValue); // Cast deriv allowed
	}
}