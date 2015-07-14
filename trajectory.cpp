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
	
	reader.read("\Bsp_1.csv", delim);
	//reader.read("\Bsp_2.csv", delim);
	//reader.read("\Bsp_3.csv", delim);
	//reader.read("\Bsp_3D.csv", delim);

	//DiscreteFrechet<2> un(reader.getData());
	//UnorderedCase<2> un(reader.getData());
	OrderedCase<2> un(reader.getData());
	//RestrictedCase<2> un(reader.getData());

	//UnorderedCase<3> un(reader.getData());
	//OrderedCase<3> un(reader.getData());
	//RestrictedCase<3> un(reader.getData());

	un.run();
	un.printResults();

	string pfad = "C:\\output\\";
	double epsilen = un.getEpsilon();
	reader.write(un.getMiddleCurve(), epsilen, pfad);

	return 0;
}
