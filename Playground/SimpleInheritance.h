#pragma once
#include <string>

class Base {
public:
	void DoNothing() = delete;
};

class Derived : public Base {
public:
	std::string DoNothing(){ return "haha"; };
};