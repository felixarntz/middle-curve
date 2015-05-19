#pragma once

#include <vector>

#include "trajectory.h"
#include "base_point.h"

using namespace std;

template<size_t T>
class BS_Point :public base_point<T>{

protected:
	int dimension = 0;
	bool mainvalue = false;
	vector<bool> boolvalues;

public:
	BS_Point(vector<int> coords) : base_point(coords){
		dimension = (int)(coords.size() / 2);
	}

	void set_boolvalues(vector<bool> boolval){
		boolvalues = boolval;
		mainvalue = false;

		for (auto it : boolval){
			if (it){
				mainvalue = it;
				break;
			}
		}
	}
	
	int getDimension(){
		return dimension;
	}

	bool getMainvalue(){
		return mainvalue;
	}

	vector<bool> getBoolvalues(){
		return boolvalues;
	}

	bool getBoolvalueAt(int i){
		return boolvalues[i];
	}
};