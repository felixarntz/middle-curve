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
	//==================================
	TrajectoryObs<double, 2> tro1;
	tro1.pos[0] = 3.0;
	tro1.pos[1] = 4.0;
	tro1.time =  0;
	
	TrajectoryObs<double, 2> tro2;
	tro2.pos[0] = 8.0;
	tro2.pos[1] = 2.0;
	tro2.time = 1;

	TrajectoryObs<double, 2> tro3;
	tro3.pos[0] = 12.0;
	tro3.pos[1] = 4.0;
	tro3.time = 2;

	TrajectoryObs<double, 2> tro4;
	tro4.pos[0] = 15.0;
	tro4.pos[1] = 10.0;
	tro4.time = 3;

	Tr2<double> tra1;
	tra1.push_back(tro1);
	tra1.push_back(tro2);
	tra1.push_back(tro3);
	tra1.push_back(tro4);
	
	//==================================
	TrajectoryObs<double, 2> tro6;
	tro6.pos[0] = 5.0;
	tro6.pos[1] = 3.0;
	tro6.time = 0;

	TrajectoryObs<double, 2> tro7;
	tro7.pos[0] = 2.0;
	tro7.pos[1] = 8.0;
	tro7.time = 1;

	TrajectoryObs<double, 2> tro8;
	tro8.pos[0] = 10.0;
	tro8.pos[1] = 12.0;
	tro8.time = 2;

	TrajectoryObs<double, 2> tro9;
	tro9.pos[0] = 12.0;
	tro9.pos[1] = 18.0;
	tro9.time = 3;

	Tr2<double> tra2;
	tra2.push_back(tro6);
	tra2.push_back(tro7);
	tra2.push_back(tro8);
	tra2.push_back(tro9);


	TrajectoryObs<double, 2> tro18;
	tro18.pos[0] = 4.0;
	tro18.pos[1] = 8.0;
	tro18.time = 2;

	TrajectoryObs<double, 2> tro19;
	tro19.pos[0] = 7.0;
	tro19.pos[1] = 12.0;
	tro19.time = 3;

	Tr2<double> tra3;
	tra3.push_back(tro18);
	tra3.push_back(tro19);
	//==================================

	//// tro is copied on push_back(), so we can edit it and push the same object again
	//tro.time = 2;
	//tr.push_back(tro);

	for (Tr2<double>::iterator tri = tra1.begin(); tri != tra1.end(); ++tri){
		std::cout << *tri << std::endl;
	}
	cout << '\n' << endl;
	for (Tr2<double>::iterator tri = tra2.begin(); tri != tra2.end(); ++tri){
		std::cout << *tri << std::endl;
	}
	cout << '\n' << endl;

	//============================================

	vector<Trajectory<double, 2>> eingabe;
	eingabe.push_back(tra1);
	eingabe.push_back(tra2);
	eingabe.push_back(tra3);

	//CSV_Reader<2> reader = CSV_Reader<2>("C:\\Users\\Marcel\\Desktop\\Bsp_1.csv");
	//reader.read();

	//DiscreteFrechet<2> un(eingabe);
	//UnorderedCase<2> un(eingabe);
	OrderedCase<2> un(eingabe);
	un.run();
	un.printResults();


	return 0;
}
