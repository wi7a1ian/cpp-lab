#include "stdafx.h"
#include "Pimpl.h"

namespace Pimpl {

	class MyClassImpl
	{
	public:
		void DoSth() { }
		void DoConst() const { }
	};

	MyClass::MyClass()
		: m_pImpl(new MyClassImpl())
	{ }

	void MyClass::DoSth()
	{
		Pimpl()->DoSth();
	}

	void MyClass::DoConst() const
	{
		Pimpl()->DoConst();
	}
}