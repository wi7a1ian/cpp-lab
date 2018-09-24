#include "stdafx.h"
#include "UnicodeStrings.h"
#include "Windows.h"

namespace StringConversionTest
{
	void UnicodeStringConversionExample() {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		//																Debug viewer:
		// char storing multibyte utf8
		std::string utf8sIn(u8"おはよう");								// -> ãŠã¯ã‚ˆã†
		// wchar_t storing multibyte utf8
		std::wstring utf8wsIn(L"おはよう");								// -> おはよう

		// BAD:
		auto utf8sOut1 = std::string(utf8wsIn.begin(), utf8wsIn.end());	// -> JoˆF
		auto utf8wsOut1 = std::wstring(utf8sIn.begin(), utf8sIn.end());	// -> ￣ﾁﾊ￣ﾁﾯ￣ﾂﾈ￣ﾁﾆ

		// GOOD and modern:
		std::string utf8sOut2 = converter.to_bytes(utf8wsIn);			// -> ãŠã¯ã‚ˆã†
		std::wstring utf8wsOut2 = converter.from_bytes(utf8sIn);		// -> おはよう

		// Not recommended types:
		std::u16string utf16sIn(u"おはよう");							// -> おはよう
		std::u32string utf32sIn(U"おはよう");							// -> おはよう
	}

	void UnicodeStringConversionExample2() {
		using convert_type = std::codecvt_utf8<wchar_t>;

		try 
		{
			std::string utf8sIn(u8"FileˇFile");
			std::wstring utf8wsOut2 = std::wstring_convert<convert_type>{}.from_bytes(utf8sIn);

			std::string utf8sIn5(u8"File¡File");
			std::wstring utf8wsOut5 = std::wstring_convert<convert_type>{}.from_bytes(utf8sIn5);

			// Lets try "File¡File"
			char utf8sIn2Raw[] = { (char)0x46, (char)0x69, (char)0x6c, (char)0x65, (char)0xa1, (char)0x46, (char)0x69, (char)0x6c, (char)0x65, '\0' };
			
			// ... success:
			std::wstring utf8wsOut3c;

			int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8sIn2Raw, static_cast<int>(strlen(utf8sIn2Raw)), NULL, 0);
			utf8wsOut3c.resize(size_needed);
			MultiByteToWideChar(CP_UTF8, 0, utf8sIn2Raw, -1, &utf8wsOut3c[0], size_needed);

			//int i = MultiByteToWideChar(CP_UTF8, 0, utf8sIn2Raw, -1, &utf8wsOut3c[0], utf8wsOut3c.length());
			assert(utf8wsOut3c[4] == 0xFF && utf8wsOut3c[4] == 0xFD); // ¡ gets replaced with special unicode char

			// ... fail
			std::wstring utf8wsOut3a = std::wstring_convert<std::codecvt_utf8<wchar_t>>{}.from_bytes(utf8sIn2Raw);
			
			// ... same fail
			std::string utf8sIn2(utf8sIn2Raw);
			std::wstring utf8wsOut3b = std::wstring_convert<convert_type>{}.from_bytes(utf8sIn2);
		}
		catch (const std::range_error&)
		{
			assert(false);
		}
	}
}