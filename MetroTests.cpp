#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <gtest/gtest.h>

using namespace std;

#include "src/Metro.h"
#include "test/MetroDomainTests.cpp"
#include "test/MetroOutputTests.cpp"
#include "test/MetroInputTests.cpp"


int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}