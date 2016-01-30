#define CPPUNIT_HAVE_NAMESPACES  1

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TextTestProgressListener.h>

#include <stdexcept>
#include <iostream>

using namespace CppUnit;
using namespace std;

int main (int argc, char* argv[])
{
	std::string testPath = (argc > 1) ? std::string(argv[1]) : "";

	// Create the event manager and test controller
	CppUnit::TestResult controller;

	// Add a listener that colllects test result
	CppUnit::TestResultCollector result;
	controller.addListener( &result );        

	// Add a listener that print dots as test run.
	CppUnit::TextTestProgressListener progress;
	controller.addListener( &progress );      

	TestRunner runner;
	TestFactoryRegistry & registry = TestFactoryRegistry::getRegistry();

	// run all tests if none specified on command line 
	Test * test_to_run = registry.makeTest();
	if (argc > 1)
		test_to_run = test_to_run->findTest(argv[1]);

	runner.addTest( test_to_run );
// 	runner.run(controller);

	try
	{
		std::cout << "Running tests"  /*<<  testPath*/ << endl;;
// 		runner.run( controller, testPath );
		runner.run(controller);

// 		std::cerr << std::endl;

		// Print test in a compiler compatible format.
		CppUnit::CompilerOutputter outputter( &result, std::cerr );
		outputter.write();                      
	}
	catch ( std::invalid_argument &e )  // Test path not resolved
	{
		std::cerr  <<  std::endl  
								<<  "ERROR: "  <<  e.what()
								<< std::endl;
		return 0;
	}

// 	result.runTest();
// 	results.printResults();
// 	runner.prin();

	return result.wasSuccessful() ? 0 : 1;
}
