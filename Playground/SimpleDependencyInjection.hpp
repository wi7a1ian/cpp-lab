#pragma once

#include <stdio.h>

#include <memory>
#include <utility>

namespace DependencyInjection
{
	using std::unique_ptr;
	using std::shared_ptr;
	using std::move;

	class SimpleDepInjectionInnerObject {
	public:
		void HelloWorld() {
			printf("Hi %s %i \n", dad.c_str(), (int)this);
		}

		void HelloWorldConst() const {
			printf("Hi %s %i \n", dad.c_str(), (int)this);
		}

		std::string dad = "dad";
	};

	class SimpleDepInjectionByValue // CONTAINS an SimpleDepInjectionByValueInner object
	{
	public:
		SimpleDepInjectionByValue()
			: inner() // Initialize inner and keep it alive untill thtis obj lives
		{
			//inner = SimpleDepInjectionByValue(); // BUG: will destroy the inner once it leaves the scope :D
			//same as auto tmp = SimpleDepInjectionByValue(); inner = tmp;

			printf("Ctor %i \n", (int)this);
		}

		SimpleDepInjectionByValue(SimpleDepInjectionInnerObject& innerObj)
			: inner(innerObj) // copy the ref value over
		{
			// We have an access to the innerObj but do not own it
		}

		~SimpleDepInjectionByValue()
		{
			printf("Dtor %i \n", (int)this);
		}

		void HelloWorld() {
			printf("Hi %i \n", (int)this);
		}

		SimpleDepInjectionInnerObject inner;
	};

	// SimpleDepInjectionByRef contains a REFERENCE to SimpleDepInjectionByValueInner object
	// We have a r/w access to the innerObj but do not own it
	class SimpleDepInjectionByRef
	{
	public:
		SimpleDepInjectionByRef(SimpleDepInjectionInnerObject& innerObj)
			: inner(innerObj)
		{
			printf("Ctor %i \n", (int)this);
		}

		~SimpleDepInjectionByRef()
		{
			printf("Dtor %i \n", (int)this);
		}

		void HelloWorld() {
			printf("Hi %i \n", (int)this);
		}

		SimpleDepInjectionInnerObject& inner;
	};

	// SimpleDepInjectionByRef contains a REFERENCE to SimpleDepInjectionByValueInner object
	// We have a read access to the innerObj but do not own it
	class SimpleDepInjectionByConstRef
	{
	public:
		SimpleDepInjectionByConstRef(const SimpleDepInjectionInnerObject& innerObj)
			: inner(innerObj)
		{
			printf("Ctor %i \n", (int)this);
		}

		void HelloWorld() {
			printf("Hi %i \n", (int)this);
		}

		~SimpleDepInjectionByConstRef()
		{
			printf("Dtor %i \n", (int)this);
		}
		const SimpleDepInjectionInnerObject& inner;
	};

	// SimpleDepInjectionByPtr contains a POINTER to a SimpleDepInjectionByPtr object
	// We have an access to the innerObj and can own it (but shouldn't!)
	// Never transfer ownership by a raw pointer (T*)
	// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#i11-never-transfer-ownership-by-a-raw-pointer-t
	class SimpleDepInjectionByPtr
	{
	public:

		SimpleDepInjectionByPtr(SimpleDepInjectionByPtr *innerObj = nullptr)
			: inner(innerObj), injected(true)
		{
			// We have an access to the innerObj and can controll it (but shouldn't!)

			printf("Ctor %i \n", (int)this);
		}

		~SimpleDepInjectionByPtr()
		{
			printf("Dtor %i \n", (int)this);

			if (injected) {
				inner = nullptr; // we are not the owners
			}
			else {
				delete inner; // we are the owners
			}
		}

		void HelloWorld() {
			printf("Hi %i \n", (int)this);
		}

		SimpleDepInjectionByPtr* inner;
		bool injected = false;
	};

	// SimpleDepInjectionByUniquePtr contains a smart ptr to a SimpleDepInjectionByPtr object
	// We steal ownership
	// https://stackoverflow.com/questions/37935393/pass-by-value-vs-pass-by-rvalue-reference
	// https://stackoverflow.com/questions/8114276/how-do-i-pass-a-unique-ptr-argument-to-a-constructor-or-a-function/8114913#8114913
	class SimpleDepInjectionByUniquePtr
	{
	public:

		SimpleDepInjectionByUniquePtr() = default;

		//SimpleDepInjectionByUniquePtr(unique_ptr<SimpleDepInjectionByUniquePtr> innerObj) : inner(move(innerObj))
		//{
		//	// innerObj will be stolen from the caller
		//	// Works only with ctor(move()), ctor(temp)
		//}

		SimpleDepInjectionByUniquePtr(unique_ptr<SimpleDepInjectionByUniquePtr> &innerObj) : inner(move(innerObj))
		{
			// innerObj will be stolen from the caller
			// Works only with ctor(innerObj)
			// Avoid! Called may not see that you will steal this object. Also, won't work with temp objects.
		}

		SimpleDepInjectionByUniquePtr(unique_ptr<SimpleDepInjectionByUniquePtr> &&innerObj) : inner(move(innerObj))
		{
			// innerObj will be stolen from the caller
			// Works only with ctor(move()), ctor(temp)
			// Works with temp objects and caller explicitly move() inner object here.
		}

		// Non-copyable class
		SimpleDepInjectionByUniquePtr(const SimpleDepInjectionByUniquePtr&) = delete;
		SimpleDepInjectionByUniquePtr& operator=(const SimpleDepInjectionByUniquePtr&) = delete;

		void HelloWorld() {
			printf("Hi %i \n", (int)this);
		}

		unique_ptr<SimpleDepInjectionByUniquePtr> inner; // will be destroyed once *this gets killed
	};

	// SimpleDepInjectionBySharedPtr contains a smart ptr to a SimpleDepInjectionBySharedPtr object
	// We all share ownership
	class SimpleDepInjectionBySharedPtr
	{
	public:

		SimpleDepInjectionBySharedPtr() = default;

		SimpleDepInjectionBySharedPtr(shared_ptr<SimpleDepInjectionBySharedPtr> innerObj) : inner(innerObj)
		{
			// innerObj wil be stolen from the caller
		}

		void HelloWorld() {
			printf("Hi %i \n", (int)this);
		}

		shared_ptr<SimpleDepInjectionBySharedPtr> inner; // will be destroyed once *this gets killed
	};

	void test();
	void test2();
	void test3();
	void test4();
	void test5();
	void test6();

}