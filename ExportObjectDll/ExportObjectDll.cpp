#include "Shared\Interfaces.h"

class DllObject : public IObject {
public:

	void UseCharVector(const std::vector<wchar_t>& string) override {
		auto* a = &string[0];
		//a = 'a';
	};

	void SetCharVector(std::vector<wchar_t>& string) override {
		auto* a = &string[0];
		*a = L'a';
	};


	void ReadString(const wchar_t* string) override {
		string = nullptr;
	};

	void SetString(wchar_t* string) override {
		int index = 0;
		string += index;
		*string = L'a';
		string[0] = L'a';
	};


	void SetCharArray(wchar_t string[]) override {
		string[0] = L'a';
	}

	virtual void ConsumeSafeString(std::unique_ptr<wchar_t[]>&& string) override {
		string[0] = L'a';
		string.release();
	}

	virtual void SetSafeString(std::unique_ptr<wchar_t[]>& string) override {
		string[0] = 'a';
	}

	virtual void ShareSafeString(std::shared_ptr<wchar_t> string) override {
		auto* a = string.get();
		a[0] = L'a';
	}

	void Release() override { 
		delete this; 
	}
};

EXTERN_C XYZAPI IObject* /*APIENTRY*/ CreateRawObject() {
	return new DllObject();
}