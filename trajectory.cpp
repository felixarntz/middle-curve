// Compile with "g++ -std=c++11 trajectory.cpp"
#pragma once

#include <vector>
#include <iostream>
#include "trajectory.h"
#include "discreteFrechet.h"
#include "unorderedCase.h"
#include "orderedCase.h"
#include "CSV_Reader.h"

int main (int argc, char** argv) {

	char delim = ',';
	CSV_Reader<2> reader;

	//DiscreteFrechet<2> un(reader.read("\Bsp_1.csv", delim));
	//DiscreteFrechet<2> un(reader.read("\Bsp_2.csv", delim));

	//UnorderedCase<2> un(reader.read("\Bsp_1.csv", delim));
	UnorderedCase<2> un(reader.read("\Bsp_2.csv", delim));

	//OrderedCase<2> un(reader.read("\Bsp_1.csv", delim));
	//OrderedCase<2> un(reader.read("\Bsp_2.csv", delim));

	un.run();
	un.printResults();


	return 0;
}
