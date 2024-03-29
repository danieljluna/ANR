#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "GordianEngine/Debug/Public/Exceptions.h"
#include "GordianEngine/Debug/Public/Asserts.h"
#include "GordianEngine/Debug/Public/LogOutputManager.h"

int main(int argc, char* argv[]) {
	// global setup...

	Gordian::GLogOutputManager.SetIsEnabled(false);

	Gordian::FAssert::OnAnyFailure = 
		[](const char* InLogText, const char* InFileName, int InLineNumber)
		{
			throw Gordian::AssertionFailure(InLogText);
		};

	int result = Catch::Session().run(argc, argv);

	// global clean-up...

	return result;
}
