#include <cppunit/extensions/HelperMacros.h>

#include "functions.h"

class BasicCase_tests : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( BasicCase_tests );
	CPPUNIT_TEST( MyTest );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();

protected:
	void MyTest();
};

CPPUNIT_TEST_SUITE_REGISTRATION( BasicCase_tests );

void BasicCase_tests::setUp()
{
}

void BasicCase_tests::MyTest()
{
	TString file1 = "example.root";
	TString file1cfg = generate_cfg_name(file1);
	TString file1exp = ".example.iecfg";
	CPPUNIT_ASSERT_EQUAL(file1exp, file1cfg);

	TString file2 = "some/dir/example.root";
	TString file2cfg = generate_cfg_name(file2);
	TString file2exp = "some/dir/.example.iecfg";
	CPPUNIT_ASSERT_EQUAL(file2exp, file2cfg);

	TString file3 = "some/dir/example.other";
	TString file3cfg = generate_cfg_name(file3);
	TString file3exp = "some/dir/.example.other.iecfg";
	CPPUNIT_ASSERT_EQUAL(file3exp, file3cfg);
}
