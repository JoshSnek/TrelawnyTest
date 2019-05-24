#include "TestBaseClass.h"
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace ::std;

namespace TrelawnyTest
{
#pragma region SuccessText
	const std::string successMessage = "\
      .o.       oooo  oooo       ooooooooooooo                        .                 ooooooooo.                                               .o8  \n\
     .888.      `888  `888       8'   888   `8                      .o8                 `888   `Y88.                                             888  \n\
    .88888.      888   888            888       .ooooo.   .oooo.o .o888oo  .oooo.o       888   .d88'  .oooo.    .oooo.o  .oooo.o  .ooooo.   .oooo888  \n\
   .8' `888.     888   888            888      d88' `88b d88(  ^8   888   d88(  ^8       888ooo88P'  `P  )88b  d88(  ^8 d88(  ^8 d88' `88b d88' `888  \n\
  .88ooo8888.    888   888            888      888ooo888 `^Y88b.    888   `^Y88b.        888          .oP^888  `^Y88b.  `^Y88b.  888ooo888 888   888  \n\
 .8'     `888.   888   888            888      888    .o o.  )88b   888 . o.  )88b       888         d8(  888  o.  )88b o.  )88b 888    .o 888   888  \n\
o88o     o8888o o888o o888o          o888o     `Y8bod8P' 8^^888P'   ^888^ 8^^888P'      o888o        `Y888^^8o 8^^888P' 8^^888P' `Y8bod8P' `Y8bod88P  \n\
";
	const std::string failureMessage = "\
ooooooooooooo                        .                 oooooooooooo            o8o  oooo                  .o8  \n\
8'   888   `8                      .o8                 `888'     `8            `^'  `888                 ^888  \n\
     888       .ooooo.   .oooo.o .o888oo  .oooo.o       888          .oooo.   oooo   888   .ooooo.   .oooo888  \n\
     888      d88' `88b d88(  ^8   888   d88(  ^8       888oooo8    `P  )88b  `888   888  d88' `88b d88' `888  \n\
     888      888ooo888 `^Y88b.    888   `^Y88b.        888    ^     .oP^888   888   888  888ooo888 888   888  \n\
     888      888    .o o.  )88b   888 . o.  )88b       888         d8(  888   888   888  888    .o 888   888  \n\
    o888o     `Y8bod8P' 8^^888P'   ^888^ 8^^888P'      o888o        `Y888^^8o o888o o888o `Y8bod8P' `Y8bod88P^ \n\
";
#pragma endregion

#pragma region helpers

	void SetConsoleColour(int colour)
	{
#ifdef _WIN32
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, colour);
#endif
	}

#pragma endregion

	TestBaseClass::TestBaseClass() :
		passed(0),
		failed(0)
	{}

	void TestBaseClass::AddTest(function<void()> test)
	{
		m_testMethods.push_back(test);
	}

	void TestBaseClass::AssertionFailure(string fileName, string functionName, int fileLine, string expected, string actual)
	{
		throw TestException(fileName, functionName, fileLine, expected, actual);
	}

	bool TestBaseClass::RunAllTests()
	{
		SetConsoleColour(12); // failure text appears red

		for (unsigned int i = 0; i < m_testMethods.size(); ++i)
		{
			Setup();
			bool testCompleted = false;
			try
			{
				auto test = m_testMethods[i];
				test();
				testCompleted = true;
			}
			catch (TestException failure)
			{
				cout << "* Test failed in file `" << failure.FileName << "` in function `" << failure.FunctionName << "` line " << failure.LineNumber << endl;
				cout << "Expected: " << failure.Expected << " Actual: " << failure.Actual << endl << endl;
			}
			catch (const exception &e)
			{
				cout << "Test encountered an unexpected exception: " << e.what() << endl;
			}
			catch (...)
			{
				cout << "Test encountered error of unknown type" << endl;
			}
			if (testCompleted)
				passed++;
			else
				failed++;
			Teardown();
		}

		if (failed == 0)
		{
			SetConsoleColour(10); // success text appears green
			cout << endl << successMessage << endl;
		}
		else
		{
			cout << endl << failureMessage << endl;
		}
		cout << "Total passed: " << passed << endl;
		cout << "Total failed: " << failed << endl;

		SetConsoleColour(11); // following text is in cyan
		return failed == 0;
	}
}