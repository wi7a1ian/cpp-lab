// AsyncIOCompletionPorts.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cassert>
#include <string>
#include <memory>
#include <algorithm>  
#include <iterator>  
#include <Windows.h>

// http://msdn.microsoft.com/en-us/library/aa365683.aspx
// http://msdn.microsoft.com/en-us/library/cc500408.aspx
// http://www.drdobbs.com/cpp/multithreaded-asynchronous-io-io-comple/201202921
// https://weblogs.asp.net/kennykerr/parallel-programming-with-c-part-4-i-o-completion-ports
// https://msdn.microsoft.com/en-us/library/windows/desktop/bb540534(v=vs.85).aspx
// !! https://www.microsoftpressstore.com/articles/article.aspx?p=2224047&seqNum=5 !!
// https://theboostcpplibraries.com/boost.asio-platform-specific-io-objects
// https://code.msdn.microsoft.com/windowsdesktop/CppAsynchronousIO-a3f7c07d/sourcecode?fileId=21543&pathId=1233555431
/*

Code Logic:

1. Create/open a file for reading and writing asynchronously by calling
CreateFile, specifying the FILE_FLAG_OVERLAPPED flag in the
dwFlagsAndAttributes parameter.

2. To queue an I/O request for a device driver, call ReadFile and WriteFile
with an OVERLAPPED structure.

3. To receive completed I/O request notifications, Windows offers four
different methods:

a) Signaling a device kernel object: Not useful for performing multiple
simultaneous I/O requests against a single device. Allows one thread to
issue an I/O request and another thread to process it. (See
WaitForDeviceObject)

b) Signaling an event kernel object: Allows multiple simultaneous I/O
requests against a single device. Allows one thread to issue an I/O request
and another thread to process it. (See: WaitForEventObject and
GetOverlappedResult)

c) Using alertable I/O: Allows multiple simultaneous I/O requests against a
single device. The thread that issued an I/O request must also process it.
(See: AlertableIO)

d) Using I/O completion ports: Allows multiple simultaneous I/O requests
against a single device. Allows one thread to issue an I/O request and
another thread to process it. This technique is highly scalable and has the
most flexibility. (See IoCompletionPort)


HANDLE CreateIoCompletionPort(
	HANDLE FileHandle,
	HANDLEExistingCompletionPort,
	ULONG_PTR  CompletionKey,
	DWORD NumberOfConcurrentThreads
);

BOOL ReadFile(
	HANDLE    File,
	LPVOID    pBuffer,
	DWORD     NumberOfBytesToRead,
	LPDWORD   pNumberOfBytesRead,
	LPOVERLAPPED  pOverlapped
);

BOOL GetQueuedCompletionStatus(
	HANDLE   CompletionPort,
	LPDWORD   pNumberOfBytes,
	PULONG_PTR   pCompletionKey,
	LPOVERLAPPED*   ppOverlapped,
	DWORD   Timeout
);

BOOL PostQueuedCompletionStatus(
	HANDLE       CompletionPort,
	DWORD   NumberOfBytesTransferred,
	ULONG_PTR    CompletionKey,
	LPOVERLAPPED pOverlapped
);

*/

int main()
{
	std::wstring filename{ L"test.dat" };
	auto numberOfThreads = 4;
	auto readBuffer = std::make_unique<wchar_t[]>(65 * 1024);
	auto readBuffer2 = std::make_unique<wchar_t[]>(65 * 1024);
	auto writeBuffer = std::make_unique<wchar_t[]>(65*1024);

	std::copy(filename.begin(), filename.end(), stdext::checked_array_iterator<wchar_t*>(writeBuffer.get(), filename.size() * sizeof(wchar_t)));

	auto file_handle = CreateFile( filename.c_str(), 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
		NULL);
	if (file_handle) {
		
		auto port_handle = CreateIoCompletionPort(file_handle, nullptr, 0, numberOfThreads);
		if (port_handle) {

			HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, _T("IOEvent"));
			if (hEvent) {

				OVERLAPPED o = { 0,0,0 };
				//o.Offset = 345; // start from 345 byte
				o.hEvent = hEvent; // register callback event, use WriteFileEx or ReadFileEx instead ;)

				DWORD bytesRead = 0;
				DWORD bytesWritten = 0;
				
				{// Writing
					auto success = WriteFile(file_handle, writeBuffer.get(), filename.size() * sizeof(wchar_t), &bytesWritten, &o); // better: WriteFileEx
					auto error = GetLastError();
					if (!success && error == ERROR_IO_PENDING) {

						WaitForSingleObject(o.hEvent, INFINITE); // blocking
						//WaitForSingleObjectEx(o.hEvent, INFINITE, true);
						//WaitForMultipleObjects(...);

						success = true;
					} // else performed synchronously or some other IO fail

					if (success) {
						// o.Internal contains the I/O error
						// o.InternalHigh contains the number of bytes transferred
						// readBuffer contains the read data
					}
					else {
						// An error occurred; see error
					}
				}

				{// Reading 1
					auto success = ReadFile(file_handle, readBuffer.get(), filename.size() * sizeof(wchar_t), &bytesRead, &o); // better: ReadFileEx
					auto error = GetLastError();
					if (!success && error == ERROR_IO_PENDING) {
						
						//WaitForSingleObject(o.hEvent, INFINITE);
						success = GetOverlappedResult(file_handle, &o, &bytesRead, true);  // blocking
					
					} // else performed synchronously or some other IO fail

					if (success) {
						// o.Internal contains the I/O error
						// o.InternalHigh contains the number of bytes transferred
						// readBuffer contains the read data
					}
					else {
						// An error occurred; see error
					}
				}

				{// Reading 2
					auto success = ReadFile(file_handle, readBuffer2.get(), filename.size() * sizeof(wchar_t), &bytesRead, &o); // better: ReadFileEx
					auto error = GetLastError();
					if (!success && error == ERROR_IO_PENDING) {

						//WaitForSingleObject(o.hEvent, INFINITE);
						//success = GetOverlappedResult(file_handle, &o, &bytesRead, true);  // blocking

						ULONG completionKey;
						OVERLAPPED* po = &o;
						success = GetQueuedCompletionStatus(port_handle, &bytesRead, &completionKey, &po, INFINITE);  // blocking

					} // else performed synchronously or some other IO fail

					if (success) {
						// o.Internal contains the I/O error
						// o.InternalHigh contains the number of bytes transferred
						// readBuffer2 contains the read data
					}
					else {
						// An error occurred; see error
					}
				}

				CloseHandle(hEvent);
			}

			CloseHandle(port_handle);
		}

		CloseHandle(file_handle);
	}

	// Validate
	//assert(std::equal(writeBuffer.get(), writeBuffer.get() + filename.size() * sizeof(wchar_t), readBuffer.get()));
	assert(readBuffer[0] == filename[0] && readBuffer[7] == *(filename.end() - 1));
	assert(readBuffer2[0] == filename[0] && readBuffer2[7] == *(filename.end()-1));

    return 0;
}

#pragma region Example1
/*

* AsyncIOGetOverlappedResult demonstrates receiving completed asynchronous
* I/O request notifications by calling GetOverlappedResult.
* GetOverlappedResult internally calls WaitForSingleObject to wait for the
* device kernel object if OVERLAPPED.hEvent is null, or to wait for
* OVERLAPPED.hEvent if it's not null. The solution combines
* WaitForDeviceObject and WaitForEventObject. It allows one thread to issue
* an I/O request and another thread to process it.
*
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.

#pragma region "Includes" 
#include <stdio.h> 
#include <tchar.h> 
#include <assert.h> 
#include <windows.h> 
#pragma endregion 


void AsyncIOGetOverlappedResult(void)
{
	HANDLE hFile;
	TCHAR szFileName[] = _T("MSDN.tmp");
	DWORD dwError;


	///////////////////////////////////////////////////////////////////////// 
	// Write file asynchronously and receive the completed asynchronous I/O  
	// request notifications by calling GetOverlappedResult. 
	//  

	// 1. Create/open a file for write asynchronously 

	_tprintf(_T("Create the file %s\n"), szFileName);

	hFile = CreateFile(szFileName,        // Name of the file 
		GENERIC_WRITE,                    // Open for writing 
		0,                                // Do not share 
		NULL,                            // Default security 
		CREATE_ALWAYS,                    // Overwrite existing 
										  // The file must be opened for asynchronous I/O by using the  
										  // FILE_FLAG_OVERLAPPED flag. 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("Could not create file w/err 0x%08lx\n"), GetLastError());
		return;
	}

	// 2. Initialize the OVERLAPPED structure 
	// To avoid the confusion of multiple asynchronous calls to the same  
	// object, all asynchronous I/O requests must specify the starting file  
	// offset in the OVERLAPPED structure. OffsetHigh and Offset indicate  
	// 64-bit offset in the file where you want the I/O operation to begin. 

	OVERLAPPED oWrite = { 0 };
	oWrite.OffsetHigh = 0;
	oWrite.Offset = 0;
	//oWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, _T("WriteEvent")); 

	// 3. Issue the Write command to write 100 bytes at the offset specified  
	// in OVERLAPPED.OffsetHigh/Offset to the file from the buffer 

	BYTE bWriteBuffer[] = "0123456789";    // 11 bytes (including '\0') 
	DWORD dwBytesWritten;
	BOOL bWriteDone = WriteFile(hFile, bWriteBuffer, 11, &dwBytesWritten, &oWrite);

	// If WriteFile returns TRUE, it indicates that the I/O request was   
	// performed synchronously. At this moment, dwBytesWritten is meaningful. 
	// See http://support.microsoft.com/kb/156932 

	// Else 

	// If the I/O request was NOT performed synchronously (WriteFile returns  
	// FALSE), check to see whether an error occurred or whether the I/O is  
	// being performed asynchronously. (GetLastError() == ERROR_IO_PENDING) 
	// At this moment, dwBytesWritten is meaningless. 
	dwError = GetLastError();
	if (!bWriteDone && (dwError == ERROR_IO_PENDING))
	{
		// The I/O is being performed asynchronously 

		// There are many things a program can do while waiting for async  
		// operations to complete, such as queuing additional operations, or  
		// doing background work. 
		// ...... 

		// Wait for the operation to complete before continuing 
		// If OVERLAPPED.hEvent is not NULL, GetOverlappedResult waits for  
		// the event object, otherwise, GetOverlappedResult waits for the  
		// device kernel object pointed by hFile. 
		if (GetOverlappedResult(hFile, &oWrite, &dwBytesWritten, TRUE))
		{
			bWriteDone = TRUE;
		}
		else
		{
			dwError = GetLastError();
		}
	}

	// 4. Handle the result 
	// The write operation was finished successfully or failed with an error 
	if (bWriteDone)
	{
		// dwBytesWritten contains the number of written bytes 
		// bWriteBuffer contains the written data 

		// Print the result 
		printf("%d bytes were written: %s\n", dwBytesWritten, bWriteBuffer);
	}
	else
	{
		// An error occurred; see dwError 

		// Print the error 
		_tprintf(_T("AsyncIOWaitForDeviceObject(Write) failed w/err 0x%08lx\n"),
			dwError);
	}

	// 5. Close the file 
	CloseHandle(hFile);


	///////////////////////////////////////////////////////////////////////// 
	// Read file asynchronously and receive the completed asynchronous I/O  
	// request notifications by calling GetOverlappedResult. 
	//  

	// 1. Open the file for read asynchronously 

	_tprintf(_T("Open the file %s\n"), szFileName);

	hFile = CreateFile(szFileName,        // Name of the file 
		GENERIC_READ,                    // Open for reading 
		FILE_SHARE_READ,                // Share reading 
		NULL,                            // Default security 
		OPEN_EXISTING,                    // Open existing 
										  // The file must be opened for asynchronous I/O by using the  
										  // FILE_FLAG_OVERLAPPED flag. 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("Could not open file w/err 0x%08lx\n"), GetLastError());
		return;
	}

	// 2. Initialize the OVERLAPPED structure 
	// To avoid the confusion of multiple asynchronous calls to the same  
	// object, all asynchronous I/O requests must specify the starting file  
	// offset in the OVERLAPPED structure. OffsetHigh and Offset indicate  
	// 64-bit offset in the file where you want the I/O operation to begin. 

	OVERLAPPED oRead = { 0 };
	oRead.OffsetHigh = 0;
	oRead.Offset = 0;
	//oRead.hEvent = CreateEvent(NULL, TRUE, FALSE, _T("ReadEvent")); 

	// 3. Issue the Read command to read 100 bytes at the offset specified  
	// in OVERLAPPED.OffsetHigh/Offset from the file to the buffer 

	BYTE bReadBuffer[100];
	DWORD dwBytesRead;
	BOOL bReadDone = ReadFile(hFile, bReadBuffer, 100, &dwBytesRead, &oRead);

	// If ReadFile returns TRUE, it indicates that the I/O request was   
	// performed synchronously. At this moment, dwBytesRead is meaningful. 
	// See http://support.microsoft.com/kb/156932 

	// Else 

	// If the I/O request was NOT performed synchronously (ReadFile returns  
	// FALSE), check to see whether an error occurred or whether the I/O is  
	// being performed asynchronously. (GetLastError() == ERROR_IO_PENDING) 
	// At this moment, dwBytesRead is meaningless. 
	dwError = GetLastError();
	if (!bReadDone && (dwError == ERROR_IO_PENDING))
	{
		// The I/O is being performed asynchronously 

		// There are many things a program can do while waiting for async  
		// operations to complete, such as queuing additional operations, or  
		// doing background work. 
		// ...... 

		// Wait for the operation to complete before continuing 
		// If OVERLAPPED.hEvent is not NULL, GetOverlappedResult waits for  
		// the event object, otherwise, GetOverlappedResult waits for the  
		// device kernel object pointed by hFile. 
		if (GetOverlappedResult(hFile, &oRead, &dwBytesRead, TRUE))
		{
			bReadDone = TRUE;
		}
		else
		{
			dwError = GetLastError();
		}
	}

	// 4. Handle the result 
	// The read operation was finished successfully or failed with an error 
	if (bReadDone)
	{
		// dwBytesRead contains the number of read bytes 
		// bBuffer contains the read data 

		// Print the result 
		printf("%d bytes were read: %s\n", dwBytesRead, bReadBuffer);
	}
	else
	{
		// An error occurred; see dwError 

		// Print the error 
		_tprintf(_T("AsyncIOWaitForDeviceObject(Read) failed w/err 0x%08lx\n"),
			dwError);
	}

	// 5. Close the file 
	CloseHandle(hFile);
}

*/
#pragma endregion

#pragma region Example2
/*

AsyncIOWaitForDeviceObject demonstrates receiving completed asynchronous
* I/O request notifications by waiting for the device kernel object. It's not
* useful for performing multiple simultaneous I/O requests against a single
* device. The solution allows one thread to issue an I/O request and another
* thread to process it.
*
* In Windows, a device kernel object can be used for thread synchronization,
* so the object can either be in a signaled or nonsignaled state. The
* ReadFile and WriteFile functions set the device kernel object to the
* nonsignaled state just before queuing the I/O request. When the device
* driver completes the request, the driver sets the device kernel object to
* the signaled state. A thread can determine whether an asynchronous I/O
* request has completed by calling either WaitForSingleObject or
* WaitForMultipleObjects.
*
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.

#pragma region "Includes" 
#include <stdio.h> 
#include <tchar.h> 
#include <assert.h> 
#include <windows.h> 
#pragma endregion 


void AsyncIOWaitForDeviceObject(void)
{
	HANDLE hFile;
	TCHAR szFileName[] = _T("MSDN.tmp");
	DWORD dwError;


	///////////////////////////////////////////////////////////////////////// 
	// Write file asynchronously and receive the completed asynchronous I/O  
	// request notifications by waiting for the device kernel object. 
	//  

	// 1. Create/open a file for write asynchronously 

	_tprintf(_T("Create the file %s\n"), szFileName);

	hFile = CreateFile(szFileName,        // Name of the file 
		GENERIC_WRITE,                    // Open for writing 
		0,                                // Do not share 
		NULL,                            // Default security 
		CREATE_ALWAYS,                    // Overwrite existing 
										  // The file must be opened for asynchronous I/O by using the  
										  // FILE_FLAG_OVERLAPPED flag. 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("Could not create file w/err 0x%08lx\n"), GetLastError());
		return;
	}

	// 2. Initialize the OVERLAPPED structure 
	// To avoid the confusion of multiple asynchronous calls to the same  
	// object, all asynchronous I/O requests must specify the starting file  
	// offset in the OVERLAPPED structure. OffsetHigh and Offset indicate  
	// 64-bit offset in the file where you want the I/O operation to begin. 

	OVERLAPPED oWrite = { 0 };
	oWrite.OffsetHigh = 0;
	oWrite.Offset = 0;

	// 3. Issue the Write command to write 100 bytes at the offset specified  
	// in OVERLAPPED.OffsetHigh/Offset to the file from the buffer 

	BYTE bWriteBuffer[] = "0123456789";    // 11 bytes (including '\0') 
	DWORD dwBytesWritten;
	BOOL bWriteDone = WriteFile(hFile, bWriteBuffer, 11, &dwBytesWritten,
		&oWrite);

	// If WriteFile returns TRUE, it indicates that the I/O request was   
	// performed synchronously. At this moment, dwBytesWritten is meaningful. 
	// See http://support.microsoft.com/kb/156932 

	// Else 

	// If the I/O request was NOT performed synchronously (WriteFile returns  
	// FALSE), check to see whether an error occurred or whether the I/O is  
	// being performed asynchronously. (GetLastError() == ERROR_IO_PENDING) 
	// At this moment, dwBytesWritten is meaningless. 
	dwError = GetLastError();
	if (!bWriteDone && (dwError == ERROR_IO_PENDING))
	{
		// The I/O is being performed asynchronously 

		// There are many things a program can do while waiting for async  
		// operations to complete, such as queuing additional operations, or  
		// doing background work. 
		// ...... 

		// Wait on the device object for the async I/O operation to complete 
		WaitForSingleObject(hFile, INFINITE);

		// oWrite.Internal contains the I/O error 
		// oWrite.InternalHigh contains the number of bytes transferred 
		dwError = oWrite.Internal;
		dwBytesWritten = oWrite.InternalHigh;

		if (SUCCEEDED(dwError))
		{
			bWriteDone = TRUE;
		}
		else
		{
			SetLastError(dwError);
		}
	}

	// 4. Handle the result 
	// The write operation was finished successfully or failed with an error 
	if (bWriteDone)
	{
		// dwBytesWritten contains the number of written bytes 
		// bWriteBuffer contains the written data 

		// Print the result 
		printf("%d bytes were written: %s\n", dwBytesWritten, bWriteBuffer);
	}
	else
	{
		// An error occurred; see dwError 

		// Print the error 
		_tprintf(_T("AsyncIOWaitForDeviceObject(Write) failed w/err 0x%08lx\n"),
			dwError);
	}

	// 5. Close the file 
	CloseHandle(hFile);


	///////////////////////////////////////////////////////////////////////// 
	// Read file asynchronously and receive the completed asynchronous I/O  
	// request notifications by waiting for the device kernel object. 
	//  

	// 1. Open the file for read asynchronously 

	_tprintf(_T("Open the file %s\n"), szFileName);

	hFile = CreateFile(szFileName,        // Name of the file 
		GENERIC_READ,                    // Open for reading 
		FILE_SHARE_READ,                // Share reading 
		NULL,                            // Default security 
		OPEN_EXISTING,                    // Open existing 
										  // The file must be opened for asynchronous I/O by using the  
										  // FILE_FLAG_OVERLAPPED flag. 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("Could not open file w/err 0x%08lx\n"), GetLastError());
		return;
	}

	// 2. Initialize the OVERLAPPED structure 
	// To avoid the confusion of multiple asynchronous calls to the same  
	// object, all asynchronous I/O requests must specify the starting file  
	// offset in the OVERLAPPED structure. OffsetHigh and Offset indicate  
	// 64-bit offset in the file where you want the I/O operation to begin. 

	OVERLAPPED oRead = { 0 };
	oRead.OffsetHigh = 0;
	oRead.Offset = 0;

	// 3. Issue the Read command to read 100 bytes at the offset specified  
	// in OVERLAPPED.OffsetHigh/Offset from the file to the buffer 

	BYTE bReadBuffer[100];
	DWORD dwBytesRead;
	BOOL bReadDone = ReadFile(hFile, bReadBuffer, 100, &dwBytesRead, &oRead);

	// If ReadFile returns TRUE, it indicates that the I/O request was   
	// performed synchronously. At this moment, dwBytesRead is meaningful. 
	// See http://support.microsoft.com/kb/156932 

	// Else 

	// If the I/O request was NOT performed synchronously (ReadFile returns  
	// FALSE), check to see whether an error occurred or whether the I/O is  
	// being performed asynchronously. (GetLastError() == ERROR_IO_PENDING) 
	// At this moment, dwBytesRead is meaningless. 
	dwError = GetLastError();
	if (!bReadDone && (dwError == ERROR_IO_PENDING))
	{
		// The I/O is being performed asynchronously 

		// There are many things a program can do while waiting for async  
		// operations to complete, such as queuing additional operations, or  
		// doing background work. 
		// ...... 

		// Wait on the device object for the async I/O operation to complete 
		WaitForSingleObject(hFile, INFINITE);

		// oRead.Internal contains the I/O error 
		// oRead.InternalHigh contains the number of bytes transferred 
		dwError = oRead.Internal;
		dwBytesRead = oRead.InternalHigh;

		if (SUCCEEDED(dwError))
		{
			bReadDone = TRUE;
		}
		else
		{
			SetLastError(dwError);
		}
	}

	// 4. Handle the result 
	// The read operation was finished successfully or failed with an error 
	if (bReadDone)
	{
		// dwBytesRead contains the number of read bytes 
		// bBuffer contains the read data 

		// Print the result 
		printf("%d bytes were read: %s\n", dwBytesRead, bReadBuffer);
	}
	else
	{
		// An error occurred; see dwError 

		// Print the error 
		_tprintf(_T("AsyncIOWaitForDeviceObject(Read) failed w/err 0x%08lx\n"),
			dwError);
	}

	// 5. Close the file 
	CloseHandle(hFile);
}

*/
#pragma endregion
