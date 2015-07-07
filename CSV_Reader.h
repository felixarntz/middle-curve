#pragma once
#include <fstream>
#include "trajectory.h"
#include <sstream>
#include <vector>
#include <map>

using namespace std;

/**
* Beschreibung: File reader zum einlesen der Trajectorien über csv datein
*
* Autoren: Felix Arntz, Marcel Stepien, Dennis Pawlowski
*
* Datum: 05.07.2015
*/

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



	void insertTrajectory(){

		multimap<string, double>::iterator it;
		TrajectoryObs<double, T> tro;
		Trajectory<double, T> tra;

		for (it = trajectoryContainer.begin(); it != trajectoryContainer.end();) {

			string key = it->first;


			if (T == 2){ //if wurde aus performance gruenden nach aussen ausgelagert
				do {//Alle Werte von einem key werden ausgegeben	

					tro.pos[0] = it->second;
					++it;
					tro.pos[1] = it->second;
					++it;
					tro.time = it->second;
					++it;

					tra.push_back(tro);

				} while (it != trajectoryContainer.end() && key == it->first);
			}
			else { //T==3		
				do {//Alle Werte von einem key werden ausgegeben				
					tro.pos[0] = it->second;
					++it;
					tro.pos[1] = it->second;
					++it;
					tro.pos[2] = it->second;
					++it;
					tro.time = it->second;
					++it;

					tra.push_back(tro);

				} while (it != trajectoryContainer.end() && key == it->first);
			}

			sort(tra.begin(), tra.end(), [](const TrajectoryObs<double, T> &a, const TrajectoryObs<double, T> &b) -> bool {return a.time < b.time; });// Sort Trajectory
			output.push_back(tra);
			tra.erase(tra.begin(), tra.end());
		}
	}

	void printTest(){
		int count = 0;
		for (const vector<TrajectoryObs<double, T>> &a : output){
			cout << endl << "Trajektorie " << count << endl << endl;
			count++;
			for (const TrajectoryObs<double, T> &b : a){
				cout << b << endl;
			}
		}
	}


public:

	vector<Trajectory<double, T>> read(const string& str, char& delim){

		ifstream filestream(str);
		filestream.exceptions(ios_base::badbit);
		string member = "", stringPoint = "";

		try{

			if (!filestream.is_open()){ throw runtime_error("File not Found"); }
			filestream.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Erste Zeile wird ignoriert

			while (getline(filestream, stringPoint, '\n')){
				{
					istringstream streamPoint(stringPoint);

					while (getline(streamPoint, member, delim)){
						speicher.push_back(member);
					}
				}
			}

		}
		catch (ios_base::failure& exc){
			cout << "badbit Error: Read error on i/o operation" << endl;
			return vector<Trajectory<double, T>>(); //Nichts
		}
		catch (exception &e){
			cout << e.what() << endl;
			return vector<Trajectory<double, T>>(); //Nichts
		}

		insertMap();
		insertTrajectory();
		printTest();
		cout << "Datei eingelesen " << endl << endl;

		return output;

	}

};