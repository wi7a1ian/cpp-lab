#include "stdafx.h"
#include "SimpleDependencyInjection.hpp"

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