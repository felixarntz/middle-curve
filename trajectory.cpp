// Compile with "g++ -std=c++11 trajectory.cpp"
#pragma once

#include <vector>
#include <iostream>
#include "trajectory.h"
#include "discreteFrechet.h"
#include "unorderedCase.h"
#include "orderedCase.h"
#include "CSV_IO.h"
#include "restrictedCase.h"

int main (int argc, char** argv) {

	char delim = ',';
	CSV_IO<2> reader;
	//CSV_IO<3> reader;

	//DiscreteFrechet<2> un(reader.read("\Bsp_1.csv", delim));
	//DiscreteFrechet<2> un(reader.read("\Bsp_2.csv", delim));
	//DiscreteFrechet<2> un(reader.read("\Bsp_3.csv", delim));

	//UnorderedCase<2> un(reader.read("\Bsp_1.csv", delim));
	//UnorderedCase<2> un(reader.read("\Bsp_2.csv", delim));
	//UnorderedCase<2> un(reader.read("\Bsp_3.csv", delim));
	//UnorderedCase<3> un(reader.read("\Bsp_3D.csv", delim));

	//OrderedCase<2> un(reader.read("\Bsp_1.csv", delim));
	//OrderedCase<2> un(reader.read("\Bsp_2.csv", delim));
	//OrderedCase<2> un(reader.read("\Bsp_3.csv", delim));
	//OrderedCase<3> un(reader.read("\Bsp_3D.csv", delim));

	//RestrictedCase<2> un(reader.read("\Bsp_1.csv", delim));
    RestrictedCase<2> un(reader.read("\Bsp_2.csv", delim));
	//RestrictedCase<2> un(reader.read("\Bsp_3.csv", delim));
	//RestrictedCase<2> un(reader.read("\Bsp_4.csv", delim));

	un.run();
	//un.printResults();

	string pfad = "C:\\output\\";
	double epsilen = un.getEpsilon();
	reader.write(un.getMiddleCurve(), epsilen, pfad);

	return 0;
}
