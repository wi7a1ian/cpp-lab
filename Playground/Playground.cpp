#include "stdafx.h"

// algorithms
#include "contains.h"
#include "minimum.h"
#include "push_back.h"
#include "select.h"
#include "enumerate.h"

// patterns
#include "Metaprogramming.h"
#include "Pimpl.h"
#include "Crtp.h"

// interesying / other
#include "IBase.h"
#include "SimpleInheritance.h"
#include "SimpleDependencyInjection.hpp"
#include "Casting.h"
#include "Concurrency.h"
#include "Benchmark.h"
#include "UnicodeStrings.h"
#include "MitigateVirtualFunctions.h"
#include "MitigateBranchPrediction.h"
#include "SimpleDependencyInjection.hpp"
#include "DelegatingConstructors.h"
#include "ConstLValueReference.h"
#include "Visitor.h"

int main()
{
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
	//StringConversionTest::UnicodeStringConversionExample2();

	BaseClassTest::BaseClassExample1();
	

	CRPTest::ForStaticInterfacesWherebaseCanBeUsed::Test();
	CRPTest::BaseForCRTP::Test();
	CRPTest::VariadicCRTP::Test();

	VisitorPattern::Test1();
	VisitorPattern::Test2();

	return 0;
}

