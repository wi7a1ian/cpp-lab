#pragma once
#include <memory>

//https://www.bfilipek.com/2018/01/pimpl.html

namespace Pimpl {

	class MyClassImpl;

	class MyClass {
		explicit MyClass();

		void DoSth();
		void DoConst() const;

	private:
		const MyClassImpl* Pimpl() const { return m_pImpl.get(); }
		MyClassImpl* Pimpl() { return m_pImpl.get(); }

		std::unique_ptr<MyClassImpl> m_pImpl;
	};

}
