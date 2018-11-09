#pragma once

#include <memory>

namespace BaseClassTest
{
	void BaseClassExample1();

	// Example 1: Pure vitual interface
	class IBase {
	public:

		// If cross-dll object passing is required, custom deleter is required
		struct SDeleter {
			void operator()(IBase* p) { if (p) p->Free(); }
		};

		using Ptr = std::unique_ptr<IBase, SDeleter>;

		// Unless a class inherits a destructor that’s already virtual, 
		// the only way to make a destructor virtual is to explicitly declare it that way.
		virtual ~IBase() = default; // make dtor virtual

		virtual void Free() = 0;
	};

	// Example 2: Abstract base class
	class Base : public IBase {
	public:
		Base() = default;  // Required

		// User-declared destructor suppresses generation of the move operations,
		// so if movability is to be supported, “ = default” often finds a second application
		Base(Base&&) = default;
		Base& operator=(Base&&) = default;

		// ... but declaring the move operations disables the copy operations, so if
		// copyability is also desired, one more round of “ = default” does the job
		Base(const Base&) = default;
		Base& operator=(const Base&) = default;
	};

	// Example 3: Base class with virtual methods
	class Parent {
	public:
		Parent() = default;  // Required

		// User-declared destructor suppresses generation of the move operations,
		// so if movability is to be supported, “ = default” often finds a second application
		Parent(Parent&&) = default;
		Parent& operator=(Parent&&) = default;

		// ... but declaring the move operations disables the copy operations, so if
		// copyability is also desired, one more round of “ = default” does the job
		Parent(const Parent&) = default;
		Parent& operator=(const Parent&) = default;

		virtual void Something();
	};

	// Implementations:

	class DerivFromIBase : public IBase
	{
	public:
		void Free() override;
		int SomeValue{ 1 };
	};

	class DerivFromBase : public Base
	{
	public:
		DerivFromBase() = default;  // Required

		void Free() override;
		
		int SomeValue{ 2 };
	};

	class DerivFromParent : public Parent
	{
	public:
		DerivFromParent() = default;  // Required

		void Something() override;

		int SomeValue{ 3 };
	};
}