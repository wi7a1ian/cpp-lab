// Playground.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "contains.h"
#include "minimum.h"
#include "push_back.h"
#include "select.h"
#include "enumerate.h"

#include "MitigateVirtualFunctions.h"
#include "MitigateBranchPrediction.h"

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

namespace ConstLValueReference
{
	void PassPointerByConstLValueReference(std::unique_ptr<Base> const &n)
	{
		// The function can access the Base via the pointer, 
		// but it cannot store it anywhere. It cannot claim ownership of it.
	}

	void PassPointerByConstLValueReference(std::unique_ptr<const Base> const &n)
	{
		// The function can access the Base via the pointer, 
		// and it cannot modify the object it points to.
	}

	void PassPointerByNonconstLValueReference(std::unique_ptr<Base> &n)
	{
		// The function may or may not claim ownership of the pointer
		// Bad choice in general.
	}

	void PassPointerByValue(std::unique_ptr<Base> n)
	{
		// We are transferring ownership of the pointer to the function / object / etc in question
		//n.reset();
	}

	void test()
	{
		auto uptr = std::make_unique<Base>();

		// The function can access the Base via the pointer, but it cannot store it anywhere.
		PassPointerByConstLValueReference(uptr);

		// The function may or may not claim ownership of the pointer, but it cannot modify the object it points to.
		//PassPointerByLValueReferenceToConstObject(std::move(const_cast<std::unique_ptr<const Base>&>(uptr)));

		// The function may or may not claim ownership of the pointer.
		PassPointerByNonconstLValueReference(uptr);

		// We are transferring ownership of the pointer to the function / object / etc in question
		//PassPointerByValue(uptr); // won't work, need to move() rvalue or use temp obj reference
		PassPointerByValue(move(uptr)); // move existing object
		PassPointerByValue(std::make_unique<Base>()); // pass temp object

		uptr = nullptr;
	}
}

namespace DependencyInjection
{
	void test() {
		auto innerObj = SimpleDepInjectionInnerObject();
		auto obj = SimpleDepInjectionByValue(innerObj);

		innerObj.HelloWorld();		// 1
		obj.HelloWorld();			// 2
		obj.inner.HelloWorld();		// 2 srsly, *this points to the same object because SimpleDepInjectionInnerObject is a value
	}

	void test2() {
		auto innerObj = SimpleDepInjectionInnerObject();
		auto obj = SimpleDepInjectionByRef(innerObj);

		innerObj.HelloWorld();		// 1
		obj.HelloWorld();			// 2
		obj.inner.HelloWorld();		// 1
	}

	void test3() {
		auto innerObj = SimpleDepInjectionInnerObject();
		auto obj = SimpleDepInjectionByConstRef(innerObj);

		innerObj.HelloWorld();			// 1
		obj.HelloWorld();				// 2
		//obj.inner.HelloWorld();		// bad
		obj.inner.HelloWorldConst();	// 1 - but you can only call const methods on this object, and you can only pass it to methods expecting a const object.
	}

	void test4() {
		auto innerObj = new SimpleDepInjectionByPtr();
		auto obj = new SimpleDepInjectionByPtr(innerObj);

		innerObj->HelloWorld();		// 1
		obj->HelloWorld();			// 2
		obj->inner->HelloWorld();	// 1

		delete obj;
		delete innerObj;
	}

	void test5() {
		auto inner_obj_1 = std::make_unique<SimpleDepInjectionByUniquePtr>();
		auto inner_obj_2 = std::make_unique<SimpleDepInjectionByUniquePtr>();

		//SimpleDepInjectionByUniquePtr obj2(obj);
		//auto obj2 = new SimpleDepInjectionByUniquePtr(obj);
		auto obj3 = std::make_unique<SimpleDepInjectionByUniquePtr>(inner_obj_1); // works with ctor(&) only since unique_ptr is non-copyable
		auto obj4 = std::make_unique<SimpleDepInjectionByUniquePtr>(std::move(inner_obj_2)); // works with ctor(value) and ctor(&&)
		auto obj5 = std::make_unique<SimpleDepInjectionByUniquePtr>(std::make_unique<SimpleDepInjectionByUniquePtr>()); // works with ctor(value) and ctor(&&)

		//inner_obj_1->HelloWorld();		// wont work because make_unique was moved (stolen ownership)
		//inner_obj_2->HelloWorld();		// wont work because make_unique was moved (stolen ownership)

		obj3->HelloWorld();			// 4
		obj3->inner->HelloWorld();	// 3

		obj4->HelloWorld();			// 6
		obj4->inner->HelloWorld();	// 5

		obj5->HelloWorld();			// 8
		obj5->inner->HelloWorld();	// 7
	}

	void test6() {
		auto obj = std::make_shared<SimpleDepInjectionBySharedPtr>();
		auto obj2 = std::make_shared<SimpleDepInjectionBySharedPtr>(obj);

		obj->HelloWorld();			// 1
		obj2->HelloWorld();			// 2
		obj2->inner->HelloWorld();	// 1
	}
}

int main()
{
	/*
	CastingTest::CastingExample();
	ConstLValueReference::test();

	DependencyInjection::test();
	DependencyInjection::test2();
	DependencyInjection::test3();
	DependencyInjection::test4();
	DependencyInjection::test5();
	DependencyInjection::test6();

	ConcurrencyTest::ConcurrencyExamples testConcurrency;
	testConcurrency.ConcurrencyExample2();
	testConcurrency.ConcurrencyExample3();
	testConcurrency.ConcurrencyExample4();
	testConcurrency.ConcurrencyExample5();

	StringConversionTest::UnicodeStringConversionExample();
	StringConversionTest::UnicodeStringConversionExample2();

	BaseClassTest::BaseClassExample1();
	*/

	CRPTest::ForStaticInterfacesWherebaseCanBeUsed::Test();
	CRPTest::BaseForCRTP::Test();
	CRPTest::VariadicCRTP::Test();

	return 0;
}

