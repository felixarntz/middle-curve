// Compile with "g++ -std=c++11 trajectory.cpp"
#pragma once

#include <vector>
#include <iostream>
#include "trajectory.h"
#include "discreteFrechet.h"
#include "unorderedCase.h"
#include "orderedCase.h"

int main (int argc, char** argv) {
	//==================================
	TrajectoryObs<double, 2> tro1;
	tro1.pos[0] = 1.0;
	tro1.pos[1] = 1.0;
	tro1.time =  0;
	
	TrajectoryObs<double, 2> tro2;
	tro2.pos[0] = 12.0;
	tro2.pos[1] = 22.0;
	tro2.time = 1;

	TrajectoryObs<double, 2> tro3;
	tro3.pos[0] = 21.0;
	tro3.pos[1] = 23.0;
	tro3.time = 2;

	TrajectoryObs<double, 2> tro4;
	tro4.pos[0] = 41.0;
	tro4.pos[1] = 14.0;
	tro4.time = 3;

	TrajectoryObs<double, 2> tro5;
	tro5.pos[0] = 19.0;
	tro5.pos[1] = 25.0;
	tro5.time = 4;

	Tr2<double> tra1;
	tra1.push_back(tro1);
	tra1.push_back(tro2);
	tra1.push_back(tro3);
	tra1.push_back(tro4);
	tra1.push_back(tro5);

	//==================================
	TrajectoryObs<double, 2> tro6;
	tro6.pos[0] = 2.0;
	tro6.pos[1] = 1.0;
	tro6.time = 0;

	TrajectoryObs<double, 2> tro7;
	tro7.pos[0] = 32.0;
	tro7.pos[1] = 21.0;
	tro7.time = 1;

	TrajectoryObs<double, 2> tro8;
	tro8.pos[0] = 52.0;
	tro8.pos[1] = 13.0;
	tro8.time = 2;

	TrajectoryObs<double, 2> tro9;
	tro9.pos[0] = 12.0;
	tro9.pos[1] = 14.0;
	tro9.time = 3;

	TrajectoryObs<double, 2> tro10;
	tro10.pos[0] = 12.0;
	tro10.pos[1] = 5.0;
	tro10.time = 4;

	TrajectoryObs<double, 2> tro11;
	tro11.pos[0] = 42.0;
	tro11.pos[1] = 16.0;
	tro11.time = 5;


	Tr2<double> tra2;
	tra2.push_back(tro6);
	tra2.push_back(tro7);
	tra2.push_back(tro8);
	tra2.push_back(tro9);
	tra2.push_back(tro10);
	tra2.push_back(tro11);

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

	//DiscreteFrechet<2> un(eingabe);
	//UnorderedCase<2> un(eingabe);
	OrderedCase<2> un(eingabe);
	un.run();
	un.printResults();


	return 0;
}

