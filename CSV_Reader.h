#pragma once

#include <iostream>
#include <fstream>
#include "trajectory.h"
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

template<size_t T>
class CSV_Reader {

private:
	ifstream file;

public:
	CSV_Reader(const string& str){

		try{
			file = ifstream(str);
		}
		catch (ios_base::failure & exc){
			cout << "failed to load csv file" << endl;
		}
	}


	vector<string> &split(const string &s, char delim, vector<string> &elems) {
		stringstream ss(s);
		string item;
		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	vector<string> split(const string &s, char delim) {
		vector<string> elems;
		split(s, delim, elems);
		return elems;
	}

	vector<Trajectory<double, T>> read(){

		string titel;
		getline(file, titel);

		vector<Trajectory<double, T>> result = vector<Trajectory<double, T>>();
		
		while (file.good()){
			string next; 
			getline(file, next);
			
			vector<string> sT = split(next, ',');

			for (auto it : sT){
				cout << "Substring: " << it;
				
				//TO DO

			}
			cout << endl;
		}
		
		return result;
	}

};