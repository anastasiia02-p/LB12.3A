#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>
using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
            const char* tempFileName = "temp.bin";

            // Створення тимчасового файлу
            ofstream tempFile(tempFileName, ios::binary);
            if (!tempFile)
            {
                Assert::Fail(L"Не вдалося створити тимчасовий файл");
            }

           
		}
	};
}
