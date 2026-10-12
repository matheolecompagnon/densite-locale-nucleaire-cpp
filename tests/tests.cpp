/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_TestMandatory_init = false;
#include "/home/matheo.glessmer/S3/INPS/prog_scientifique_jmm/tests/test_mandatory.h"

static TestMandatory suite_TestMandatory;

static CxxTest::List Tests_TestMandatory = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestMandatory( "tests/test_mandatory.h", 19, "TestMandatory", suite_TestMandatory, Tests_TestMandatory );

static class TestDescription_suite_TestMandatory_testMandatory0 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestMandatory_testMandatory0() : CxxTest::RealTestDescription( Tests_TestMandatory, suiteDescription_TestMandatory, 22, "testMandatory0" ) {}
 void runTest() { suite_TestMandatory.testMandatory0(); }
} testDescription_suite_TestMandatory_testMandatory0;

static class TestDescription_suite_TestMandatory_testMandatory1 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestMandatory_testMandatory1() : CxxTest::RealTestDescription( Tests_TestMandatory, suiteDescription_TestMandatory, 48, "testMandatory1" ) {}
 void runTest() { suite_TestMandatory.testMandatory1(); }
} testDescription_suite_TestMandatory_testMandatory1;

static class TestDescription_suite_TestMandatory_testMandatory2 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestMandatory_testMandatory2() : CxxTest::RealTestDescription( Tests_TestMandatory, suiteDescription_TestMandatory, 76, "testMandatory2" ) {}
 void runTest() { suite_TestMandatory.testMandatory2(); }
} testDescription_suite_TestMandatory_testMandatory2;

static class TestDescription_suite_TestMandatory_testMandatory3 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestMandatory_testMandatory3() : CxxTest::RealTestDescription( Tests_TestMandatory, suiteDescription_TestMandatory, 106, "testMandatory3" ) {}
 void runTest() { suite_TestMandatory.testMandatory3(); }
} testDescription_suite_TestMandatory_testMandatory3;

#include "/home/matheo.glessmer/S3/INPS/prog_scientifique_jmm/tests/test_poly.h"

static TestPoly suite_TestPoly;

static CxxTest::List Tests_TestPoly = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestPoly( "tests/test_poly.h", 19, "TestPoly", suite_TestPoly, Tests_TestPoly );

static class TestDescription_suite_TestPoly_testHermite : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestPoly_testHermite() : CxxTest::RealTestDescription( Tests_TestPoly, suiteDescription_TestPoly, 27, "testHermite" ) {}
 void runTest() { suite_TestPoly.testHermite(); }
} testDescription_suite_TestPoly_testHermite;

static class TestDescription_suite_TestPoly_testLaguerre : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestPoly_testLaguerre() : CxxTest::RealTestDescription( Tests_TestPoly, suiteDescription_TestPoly, 110, "testLaguerre" ) {}
 void runTest() { suite_TestPoly.testLaguerre(); }
} testDescription_suite_TestPoly_testLaguerre;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
