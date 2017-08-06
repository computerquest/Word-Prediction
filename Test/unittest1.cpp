#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include "../tempDelete/MasterResource.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
		
	TEST_CLASS(MasterResourceTest)
	{
	public:
		TEST_METHOD(findWordTest)
		{
			MasterResource master;
			
			string name = ((master.findWord("he")).at(0)).name;
			
			if (name == "he") {
				Assert::IsTrue(true);
			}
			else {
				Assert::IsFalse(false);
			}
		}

		TEST_METHOD(findNGramPTest)
		{
			MasterResource master;

			string name = ((master.findNGramP("he")).at(0))->subject.name;

			if (name == "he") {
				Assert::IsTrue(true);
			}
			else {
				Assert::IsFalse(false);
			}
		}

		TEST_METHOD(findDoubleTest)
		{
			MasterResource master;

			string name = master.findDouble(master.findWord("he").at(0), "said").name;

			if (name == "he") {
				Assert::IsTrue(true);
			}
			else {
				Assert::IsFalse(false);
			}
		}
	};
