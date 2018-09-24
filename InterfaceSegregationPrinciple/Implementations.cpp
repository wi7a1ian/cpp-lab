#include "stdafx.h"

IEnumerable<IAttribute> NtfsFile::GetAttributeEnumerator() const
{
	return IEnumerable<IAttribute>();
}

IEnumerable<IStream> NtfsFile::GetStreamEnumerator()
{
	return IEnumerable<IStream>();
}

void NtfsFile::DoSoemthingElseWithFile()
{
	//std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point();
}
