// ArrayView.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	const int data[] = { 1, 2, 3, 4, 5 };

	ArrayView<int> arr1(data, 3); // So arr1 is 1, 2, 3
	ArrayView<int> arr2(data, 4); // And arr2 is 1, 2, 3, 4

	assert(arr1 != arr2);     // Obviously they can't be equal
	assert(arr1 < arr2);      // arr1 is lexicographically less than arr2
	assert(!arr1.empty());    // Surely arr1 can't be empty
	assert(arr1.size() == 3); // Yes, 3 elements is what we expect

	assert(arr1[0] == 1); // Let's make sure that the right elements are present
	assert(arr1[1] == 2);
	assert(arr1[2] == 3);

	arr1.swap(arr2); // Same as doing swap(arr1, arr2);

	assert(arr1.size() == 4); // Now arr1 should be as long as what arr2 was.

	StringView str1("hello");
	StringView str2("hello world!", 5); // It's like "hello world!", only without the " world!" part

	assert(str1 == str2);                      // should be equal
	assert(str1 < StringView("hello world!")); // lexicographically less 
	assert(str1 > StringView("abc"));          // lexicographically greater
	assert(str1 < StringView("xyz"));          // yada yada

	WStringView str3(L"hello");
	WStringView str4(L"hello world!", 5); // It's like "hello world!", only without the " world!" part

	assert(str3 == str4);                        // should be equal
	assert(str3 < WStringView(L"hello world!")); // lexicographically less 
	assert(str3 > WStringView(L"abc"));          // lexicographically greater
	assert(str3 < WStringView(L"xyz"));          // yada yada

	return 0;
}

