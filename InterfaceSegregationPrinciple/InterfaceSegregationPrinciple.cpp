// InterfaceSegregationPrinciple.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// https://stackoverflow.com/questions/500493/c-equivalent-of-instanceof
// https://en.wikipedia.org/wiki/Visitor_pattern
// http://en.cppreference.com/w/cpp/language/typeid
// http://en.cppreference.com/w/cpp/language/dynamic_cast
// https://msdn.microsoft.com/en-us/library/95b534kw.aspx
// https://aberer.io/software/eradication-of-raw-pointer-in-c++11/

//template<typename T, typename K>
//inline bool isType(const K &k) {
//	return typeid(T).hash_code() == typeid(k).hash_code();
//}

//// http://www.cplusplus.com/reference/type_traits/is_base_of/
//template<typename Base, typename T>
//inline bool instanceof(const T*) {
//	return std::is_base_of<Base, T>::value;
//}
//
//// http://www.cplusplus.com/reference/type_traits/is_convertible/
//template<typename Base, typename T>
//inline bool instanceof2(const T*) {
//	return std::is_convertible<Base, T>::value;
//}
//
//// http://www.cplusplus.com/reference/type_traits/is_convertible/
//template<typename Base, typename T>
//inline bool instanceof2_1(const T&) {
//	return std::is_convertible<Base, T>::value;
//}

template<typename Base, typename T>
inline bool implements_1(const T *ptr) {
	return dynamic_cast<const Base*>(ptr) != nullptr;
}

template<typename Base, typename T>
inline bool implements_2(const T &ptr) {
	return dynamic_cast<const Base*>(&ptr) != nullptr;
}

template<typename TTo, typename TFrom>
inline std::optional<TTo*> implements_3(TFrom &ptr) {
	try {
		return dynamic_cast<TTo*>(&ptr);
	}
	catch (std::bad_cast&) {
		return {};
	}
}

template<typename TTo, typename TFrom>
inline std::optional<std::reference_wrapper<TTo>> implements_4(TFrom &ptr) {
	try {
		return dynamic_cast<TTo&>(ptr);
	}
	catch (std::bad_cast&) {
		return {};
	}
}


int main()
{
	typedef std::chrono::microseconds TimeUnit;
	typedef std::chrono::high_resolution_clock Clock;
	std::chrono::time_point< Clock > start_time, stop_time;
	
	//--------------- temp
	//# 1
	{
		NtfsFile d1, d2;
		std::vector<std::reference_wrapper<IObject> > refVec;
		refVec.push_back(d1);
		refVec.push_back(d2);
		d1.DoSoemthingElseWithFile();
	}
	//# 2
	{
		auto d1 = std::make_unique<NtfsFile>();
		auto d2 = std::make_unique<NtfsFile>();
		std::vector<std::unique_ptr<IObject> > uniquePtrVec;
		uniquePtrVec.push_back(std::move(d1));
		uniquePtrVec.push_back(std::move(d2));
		//d1->DoSoemthingElseWithFile();// : Access violation reading location 
		//auto d3 = std::move(uniquePtrVec.at(0)); // CAREFUL uniquePtrVec.size() = 2 !
		auto d3 = std::move(uniquePtrVec.front());
		auto d4 = std::move(uniquePtrVec.back());
		auto c = uniquePtrVec.size();
	}
	//# 3
	{
		auto d1 = std::make_unique<NtfsFile>();
		auto d2 = std::make_unique<NtfsFile>();
		std::queue<std::unique_ptr<IObject> > uniquePtrQ;
		uniquePtrQ.push(std::move(d1));
		uniquePtrQ.push(std::move(d2));
		//d1->DoSoemthingElseWithFile();// : Access violation reading location 
		auto d3 = std::move(uniquePtrQ.front());
		uniquePtrQ.pop();
		auto d4 = std::move(uniquePtrQ.front()); // not back() !
		uniquePtrQ.pop();
		auto c = uniquePtrQ.size();
	}
	//---------------


	std::unique_ptr<IObject> f = std::make_unique<NtfsFile>();
	CopyOptions options{ f.get(), *f, f };
	CopyOptions options2{ nullptr, NtfsFile(), std::unique_ptr<IObject>() };
	std::cout << "Is false: " << (bool)options2.optional << std::endl;


	NtfsFile obj;
	IObject* base_ptr = &obj;
	IObject& base_ref = obj;

	size_t cycles = 100 * 1000 * 1000 * 1000;

	{
		std::cout << "No cast \t";
		start_time = Clock::now();

		for (size_t i = 0; i < cycles; ++i) {
			NtfsFile obj;
			IObject* base_ptr = &obj;
			IObject& base_ref = obj;
			obj.DoSoemthingElseWithFile();
		}

		stop_time = Clock::now();
		std::cout << std::chrono::duration_cast<TimeUnit>(stop_time - start_time).count() << std::endl;
	}

	{
		std::cout << "(IFile*)(base_ptr) \t";
		start_time = Clock::now();

		for (size_t i = 0; i < cycles; ++i) {
			auto casted = (IFile*)(base_ptr);
			casted->DoSoemthingElseWithFile();
		}

		stop_time = Clock::now();
		std::cout << std::chrono::duration_cast<TimeUnit>(stop_time - start_time).count() << std::endl;
	}

	{
		std::cout << "static_cast<IFile*>(base_ptr) \t";
		start_time = Clock::now();

		for (size_t i = 0; i < cycles; ++i) {
			auto casted = static_cast<IFile*>(base_ptr);
			casted->DoSoemthingElseWithFile();
		}

		std::cout << std::chrono::duration_cast<TimeUnit>(Clock::now() - start_time).count() << std::endl;
	}

	{
		std::cout << "static_cast<IFile&>(base_ref) \t";
		start_time = Clock::now();

		for (size_t i = 0; i < cycles; ++i) {
			auto& casted = static_cast<IFile&>(obj);
			casted.DoSoemthingElseWithFile();
		}

		std::cout << std::chrono::duration_cast<TimeUnit>(Clock::now() - start_time).count() << std::endl;
	}

	{
		std::cout << "dynamic_cast<IFile&>(base_ref) \t" << "\t";
		start_time = Clock::now();

		for (size_t i = 0; i < cycles; ++i) {
			try {
				auto& casted = dynamic_cast<IFile&>(base_ref);
				casted.DoSoemthingElseWithFile();
			}
			catch (const std::bad_cast& e) {
				// Cast failed
			}
		}

		stop_time = Clock::now();
		std::cout << std::chrono::duration_cast<TimeUnit>(stop_time - start_time).count() << std::endl;
	}

	{
		std::cout << "dynamic_cast<IFile*>(base_ptr) \t" << "\t";
		start_time = Clock::now();

		for (size_t i = 0; i < cycles; ++i) {
			if (auto casted = dynamic_cast<IFile*>(base_ptr)) {
				casted->DoSoemthingElseWithFile();
			}
		}

		stop_time = Clock::now();
		std::cout << std::chrono::duration_cast<TimeUnit>(stop_time - start_time).count() << std::endl;
	}

	{
		std::cout << "base_ref.IsOfType(ObjectTypeFlag::File) \t" << "\t";
		start_time = Clock::now();

		for (size_t i = 0; i < cycles; ++i) {

			if (base_ref.IsOfType(ObjectTypeFlag::File)) {
				auto& casted = static_cast<IFile&>(base_ref);
				casted.DoSoemthingElseWithFile();
			}
		}

		stop_time = Clock::now();
		std::cout << std::chrono::duration_cast<TimeUnit>(stop_time - start_time).count() << std::endl;
	}

	{
		std::cout << "base_ref.implements<IFile>() \t" ;
		start_time = Clock::now();

		for (size_t i = 0; i < cycles; ++i) {
			if (base_ref.implements<IFile>()) {
				auto& casted = static_cast<IFile&>(base_ref);
				casted.DoSoemthingElseWithFile();
			}
		}

		stop_time = Clock::now();
		std::cout << std::chrono::duration_cast<TimeUnit>(stop_time - start_time).count() << std::endl;
	}

	{
		std::cout << "implements_1<IFile>(base_ptr) \t" ;
		start_time = Clock::now();

		for (size_t i = 0; i < cycles; ++i) {
			if (implements_1<IFile>(base_ptr)) {
				auto& casted = static_cast<IFile&>(base_ref);
				casted.DoSoemthingElseWithFile();
			}
		}

		stop_time = Clock::now();
		std::cout << std::chrono::duration_cast<TimeUnit>(stop_time - start_time).count() << std::endl;
	}

	{
		std::cout << "implements_2<IFile>(base_ref) \t";
		start_time = Clock::now();

		for (size_t i = 0; i < cycles; ++i) {
			if (implements_2<IFile>(base_ref)) {
				auto& casted = static_cast<IFile&>(base_ref);
				casted.DoSoemthingElseWithFile();
			}
		}

		stop_time = Clock::now();
		std::cout << std::chrono::duration_cast<TimeUnit>(stop_time - start_time).count() << std::endl;
	}

	{
		std::cout << "implements_3<IFile>(base_ref) \t";
		start_time = Clock::now();

		for (size_t i = 0; i < cycles; ++i) {
			if (auto casted = implements_3<IFile>(base_ref)) {
				(*casted)->DoSoemthingElseWithFile();
			}
		}

		stop_time = Clock::now();
		std::cout << std::chrono::duration_cast<TimeUnit>(stop_time - start_time).count() << std::endl;
	}

	{
		std::cout << "implements_4<IFile>(base_ref) \t";
		start_time = Clock::now();

		for (size_t i = 0; i < cycles; ++i) {
			if (auto casted = implements_4<IFile>(base_ref)) {
				(*casted).get().DoSoemthingElseWithFile();
			}
		}

		stop_time = Clock::now();
		std::cout << std::chrono::duration_cast<TimeUnit>(stop_time - start_time).count() << std::endl;
	}

	std::cin.get();
    return 0;
}

