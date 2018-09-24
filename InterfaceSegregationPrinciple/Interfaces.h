#pragma once

//----------------------------Generic-----------------------------------------------

class IBase {
public:
	struct SDeleter {
		void operator()(IBase* p) { if (p) p->Free(); }
	};

public:
	virtual ~IBase() {}

	virtual void Free() = 0;
};



// https://www.cs.northwestern.edu/~riesbeck/programming/c++/stl-iterator-define.html
template<typename T>
class IEnumerable {
private:
	using SomeInternalList = std::vector<T>;
	SomeInternalList internalList;
public:
	using iterator = typename SomeInternalList::iterator;
	using const_iterator = typename SomeInternalList::const_iterator;

	iterator begin() { return internalList.begin(); }
	iterator end() { return internalList.end(); }
};

//-----------------------------App specific-----------------------------------------

// Proto
enum /*class*/ ObjectTypeFlag {
	Unknown		= 0,
	File		= 1 << 0,
	Stream		= 1 << 1,
	Attribute	= 1 << 2,
};

class IObject : public IBase {
public:
	using Ptr = std::unique_ptr<IObject>;

	ObjectTypeFlag type{ ObjectTypeFlag::Unknown };

	// TODO: common stuff

	// Proto
	bool IsOfType(ObjectTypeFlag flags) { return type & flags != 0; }
	
	template<typename T>
	inline bool implements() { return (dynamic_cast<T*>(this) != nullptr); }
};

class ICopyDestination : public IBase {
public:
	using Ptr = std::unique_ptr<ICopyDestination, SDeleter>;
};

class ICopySource : public IBase {
public:
	using Ptr = std::unique_ptr<ICopySource, SDeleter>;

	virtual const IObject& GetRootObject() = 0;
};

struct CopyOptions
{
	IObject* optionalButRawAndUgly = nullptr;
	IObject& obligatory;
	std::unique_ptr<IObject>& optional;
};

class ICopyEngine : public IBase {
public:
	using Ptr = std::unique_ptr<ICopyEngine>;

	virtual int	Process(const ICopySource& object, ICopyDestination& destination /*, CopyOptions& options*/) = 0;
};

//------------------------------Non-objects-----------------------------------------

class IStream : public IBase {
public:
	using Ptr = std::unique_ptr<IStream, SDeleter>;

	virtual std::ostream Read() = 0;
	virtual std::istream Write() = 0;
};

class IAttribute : public virtual IBase {
public:
	using Ptr = std::unique_ptr<IAttribute, SDeleter>;

	virtual const std::pair<std::wstring, std::wstring> ReadKVPair() = 0;
};

//------------------------------Generic roles----------------------------------------

class IContainerable {
public:
	virtual IEnumerable<IObject> GetChildEnumerator() = 0;
};

class IStreamable {
public:
	virtual IEnumerable<IStream> GetStreamEnumerator() = 0;

};

class IAttributable {
public:
	virtual IEnumerable<IAttribute> GetAttributeEnumerator() const = 0;
};

class IDecryptable {
public:
	virtual const IStream& Decrypt() = 0;
};

//-------------------------------Objects-------------------------------------------

class IFile : public IObject, public IAttributable, public IStreamable {
public:
	virtual void DoSoemthingElseWithFile() = 0;
};

class IDirectory : public IObject, public IContainerable, public IAttributable, public IStreamable {
public:
	virtual void DoSoemthingElseWithDir() = 0;
};