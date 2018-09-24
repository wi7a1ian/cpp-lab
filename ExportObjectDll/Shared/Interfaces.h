#pragma once

#include <memory>
#include <vector>

/*#define APIENTRY __stdcall*/

#if defined(XYZLIBRARY_EXPORT)
#   define XYZAPI __declspec(dllexport)
#else
#   define XYZAPI __declspec(dllimport)
#endif

#ifdef __cplusplus
#   define EXTERN_C extern "C"
#else
#   define EXTERN_C
#endif


class IBase {
public:
	struct SDeleter {
		void operator()(IBase* p) { if (p) p->Release(); }
	};

public:
	virtual ~IBase() {}
	virtual void Release() = 0;
};

class IObject : public IBase {
public:
	using Ptr = std::unique_ptr<IObject, SDeleter>;

	virtual void UseCharVector(const std::vector<wchar_t>& string) = 0; // Bad idea because std objects may be implemented differently
	virtual void SetCharVector(std::vector<wchar_t>& string) = 0; // Bad idea because std objects may be implemented differently

	virtual void ReadString(const wchar_t* string) = 0;
	virtual void SetString(wchar_t* string) = 0;

	virtual void SetCharArray(wchar_t string[]) = 0;

	virtual void ConsumeSafeString(std::unique_ptr<wchar_t[]>&& string) = 0;
	virtual void SetSafeString(std::unique_ptr<wchar_t[]>& string) = 0;
	virtual void ShareSafeString(std::shared_ptr<wchar_t> string) = 0;
};

EXTERN_C XYZAPI IObject* /*APIENTRY*/ CreateRawObject();
inline IObject::Ptr CreateObject() { return IObject::Ptr(CreateRawObject()); }
