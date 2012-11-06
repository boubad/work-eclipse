//============================================================================
// Name        : teststathdf5.cpp
// Author      : Boubacar Diarra
// Version     :
// Copyright   : boubadiarra@gmail.com
// Description : Hello World in C++, Ansi-style
//============================================================================
#ifndef NO_GTEST
///////////////////////////////
#include <gtest/gtest.h>
//////////////////////////
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	  return RUN_ALL_TESTS();
}/* main */
#else
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

int main( int /*argc */, char ** /*argv */)
{
  CppUnit::TextUi::TestRunner runner;
  CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
  runner.addTest( registry.makeTest() );
  bool wasSuccessful = runner.run( "", false );
  return (!wasSuccessful);
}
#endif // NO_GTEST
