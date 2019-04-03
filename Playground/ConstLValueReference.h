#pragma once

#include <memory>
#include "IBase.h"

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