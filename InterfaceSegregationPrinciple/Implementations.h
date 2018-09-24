#pragma once

#include "Interfaces.h"

class NtfsFile : public IFile {
public:
	NtfsFile(){
		type = ObjectTypeFlag::File;
	}; // stream, device+sector, asyncIO or FR data here

	// Inherited via IFile
	IEnumerable<IAttribute> GetAttributeEnumerator() const override;
	IEnumerable<IStream> GetStreamEnumerator() override;
	void DoSoemthingElseWithFile() override;
	
	// Inherited via IBase
	void	Free() override { delete this; }
};