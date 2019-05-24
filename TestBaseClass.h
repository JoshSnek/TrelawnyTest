#pragma once

#include <functional>
#include <vector>
#include <sstream>

namespace TrelawnyTest
{
	class TestException : public std::exception
	{
	public:
		TestException(std::string fileName, std::string functionName, int fileLine, std::string expected, std::string actual) :
			FileName(fileName),
			FunctionName(functionName),
			LineNumber(fileLine),
			Expected(expected),
			Actual(actual)
		{}

		const std::string FileName;
		const std::string FunctionName;
		const int LineNumber;
		const std::string Expected;
		const std::string Actual;
	};

	class TestBaseClass
	{
	public:
		TestBaseClass();
		virtual ~TestBaseClass() {};

		bool RunAllTests();

	protected:
		void AddTest(std::function<void()>test);

		virtual void Setup() {}
		virtual void Teardown() {}

		// WARNING: This function is intended to be used by the test framework.
		// It is strongly recommended that you do not call this function directly and instead use one of the ASSERT macros provided.
		void AssertionFailure(std::string fileName, std::string functionName, int fileLine, std::string expected, std::string actual);

	private:
		std::vector<std::function<void()>> m_testMethods;

		unsigned int passed;
		unsigned int failed;
	};

#pragma region AssertMacros

#define ASSERT_EQ(expected, actual) { if (expected != actual) \
	AssertionFailure(__FILE__, __FUNCTION__, __LINE__,\
 ((std::ostringstream&)(std::ostringstream() << expected)).str(),\
 ((std::ostringstream&)(std::ostringstream() << actual)).str() ); }

#define ASSERT_TRUE(expectTrue) { ASSERT_EQ(true, (expectTrue)); }

#define ASSERT_FALSE(expectFalse) { ASSERT_EQ(false, (expectFalse)); }

#pragma endregion
}
