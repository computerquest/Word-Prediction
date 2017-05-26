#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

	TEST_CLASS(Stufffffff)
	{
	public:

		//https://blogs.msdn.microsoft.com/jsocha/2010/11/19/writing-unit-tests-in-visual-studio-for-native-c/
		TEST_METHOD(stuff)
		{
			Assert::AreEqual(1, 1);
			// TODO: Your test code here
		}
		TEST_METHOD(stuffA)
		{
			Assert::AreEqual(0, 1);
			// TODO: Your test code here
		}

		TEST_METHOD(stuffB)
		{
			Assert::AreEqual(0, 1);
			// TODO: Your test code here
		}

	};
