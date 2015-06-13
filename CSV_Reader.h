#pragma once
#include <fstream>
#include "trajectory.h"
#include <sstream>
#include <vector>
#include <map>

using namespace std;

template<size_t T>
class CSV_Reader {

private:

	vector<string> speicher;
	multimap<string, double> trajectoryContainer;
	vector<Trajectory<double, T>> output;

	void insertMap(){

		int choiceTrajectory = T + 2;

		for (int i = choiceTrajectory - 1; i < speicher.size(); i += choiceTrajectory){
			for (int j = i - (choiceTrajectory - 1); j < i; j++){
				trajectoryContainer.insert(pair<string, double>(speicher[i], stringDoubleConverter(speicher[j])));
			}
		}
	}

	double stringDoubleConverter(string wertString){
		double number = 0.0;
		string temp = wertString.substr(1, wertString.size() - 2);
		istringstream(temp) >> number;
		return number;
	}

	bool greatherThan(const TrajectoryObs<double, T> &a, const TrajectoryObs<double, T> &b){
		return (a.time < b.time);
	}

	void insertTrajectory(){

		multimap<string, double>::iterator it;
		TrajectoryObs<double, T> tro;

		for (it = trajectoryContainer.begin(); it != trajectoryContainer.end();) {
			string key = it->first;

			if (T == 2){ //if wurde aus performance gruenden nach aussen ausgelagert
				Tr2<double> tra2Dim;

				do {//Alle Werte von einem key werden ausgegeben	

					tro.pos[0] = it->second;
					++it;
					tro.pos[1] = it->second;
					++it;
					tro.time = it->second;
					++it;

					tra2Dim.push_back(tro);

				} while (it != trajectoryContainer.end() && key == it->first);

				//sort(tra2Dim.begin(), tra2Dim.end(), greatherThan);

				output.push_back(tra2Dim); //Trajektorie
				tra2Dim.erase(tra2Dim.begin(), tra2Dim.end());
			}
			else { //T==3
				Tr3<double> tra3Dim;

				do {//Alle Werte von einem key werden ausgegeben				;
					tro.pos[0] = it->second;
					++it;
					tro.pos[1] = it->second;
					++it;
					tro.pos[2] = it->second;
					++it;
					tro.time = it->second;
					++it;

					//tra3Dim.push_back(tro);

				} while (it != trajectoryContainer.end() && key == it->first);

				//	output.push_back(tra3Dim);
				//	tra3Dim.erase(tra3Dim.begin(), tra3Dim.end());
			}
		}
	}


public:
	vector<Trajectory<double, T>> read(const string& str, char& delim){

		ifstream filestream;
		filestream.exceptions(ios_base::badbit); //Catch fall prueffen !!!!!		
		string member = "", stringPoint = "";

		try{

			filestream.open(str);
			filestream.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Erste Zeile wird ignoriert

			while (getline(filestream, stringPoint, '\n')){
				{
					istringstream streamPoint(stringPoint);

					while (getline(streamPoint, member, delim)){
						speicher.push_back(member);
					}
				}
			}
			filestream.close();
		}
		catch (ios_base::failure& exc){
			cout << "File not found" << endl;
			return vector<Trajectory<double, T>>(); //Nichts
		}

		insertMap();
		insertTrajectory();

		return output;

	}

};