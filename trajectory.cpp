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

	RestrictedCase<2> un(reader.read("\Bsp_1.csv", delim));
    //RestrictedCase<2> un(reader.read("\Bsp_2.csv", delim));

	un.run();
	//un.printResults();

	/*string pfad = "C:\\output\\";
	double epsilen = un.getEpsilon();
	reader.write(un.getMiddleCurve(), epsilen, pfad);*/

	//vector<FS_Point<2>> temp = un.getPath();
	//vector<BS_Point<2>> temp = un.getPath();

    vector<TrajectoryObs<double, 2>> temp = un.getMiddleCurve();
	double epsilon = un.getEpsilon();

	
	for (auto it : temp){
		cout << "(" << it.pos[0] << ", " << it.pos[1] << ")" << endl;
		//cout << it.pos[0] << ", " << it.pos[1] << ", " << it.pos[2] << endl;

	}

	cout << "Epsilon: " << epsilon << endl;
	


	/*for (int i = 0; i < temp.size(); i++){
		//cout << i << " (" << temp[i].get_center_point().pos[0] << "," << temp[i].get_center_point().pos[1] << ") | " << temp[i].get_center_distance() << endl;
		cout << i << " (" << temp[i].get_mainvalue() << ") | " << endl;
	}*/


	return 0;
}
