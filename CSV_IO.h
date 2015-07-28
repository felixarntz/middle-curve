#pragma once
#include <fstream>
#include "trajectory.h"
#include <sstream>
#include <vector>
#include <map>

using namespace std;

/*! \brief  File reader und writer zum Ein- und Auslesen der Trajectorien ueber csv datein
*
* File reader und writer zum Ein- und Auslesen der Trajectorien ueber csv datein
*
*/

/**
*
* Autoren: Felix Arntz, Marcel Stepien, Dennis Pawlowski
*
* Datum: 05.07.2015
*/

template<size_t T>
class CSV_IO {

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
		long double number = 0.0;
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
                    tro.trajectoryName = key;

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
                    tro.trajectoryName = key;

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
			cout << endl << "Trajectorie " << count << endl << endl;
			count++;
			
			for (const TrajectoryObs<double, T> &b : a){
				cout << b << endl;
			}
			cout << endl;
			
		}
	}


public:

	/**
	* Liest eine .csv Datei mit Trajectorien ein. 
	* Erkennt auch die Trajektorien wenn sie innerhalb der csv Datei durcheinder aufgelistet sind.
	*
	* str - der Pfad zu der Datei (inclusive datei)
	* delim - das veeinbarte delimiter synbol der csv datei (Standart als ',' setzen)
	*/
	int read(const string& str, char& delim){

		ifstream filestream(str);
		filestream.exceptions(ios_base::badbit);
		string member = "", stringPoint = "";

		try{

			if (!filestream.is_open()){ throw runtime_error("File not found"); }
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
			return 0; //Nichts
		}
		catch (exception &e){
			cout << e.what() << endl;
			return 0; //Nichts
		}

		insertMap();
		cout << "File read " << endl << endl;
        speicher.erase(speicher.begin(), speicher.end());

		return 1;

	}

	/**
	* Die Trajectorien werden nach der Time aufsteigend Sortiert zurückgegeben.
	*
	* return - die eingelesenen Daten als Vector verpackt von Trajectories
	*/
	vector<Trajectory<double, T>> getData(){
		insertTrajectory();
		cout << "Print data " << endl;
		cout << "Number of Trajectory: " << output[0].size() << endl;
		printTest();
		return output;
	}

	/**
	* Schreibt eine .csv Datei mit Trajectorien aus
	* 
	* middleCurve - die erechnete Mittelkurve
	* epsilon - der epsilonwert aus der Berechnung
	* path - der Speicherpfad zu der Datei (exclusive datei)
	*/
	int write(vector<TrajectoryObs<double, T>>& middleCurve, double& epsilon, string& path){

		ofstream filestreamOut;
        filestreamOut.precision(15);
		filestreamOut.exceptions(ios_base::failbit | ios_base::badbit);

		try{
			filestreamOut.open(path + "middleCurve.csv", ios_base::trunc);
			if (T == 2){
				filestreamOut << "\"" << "x" << "\"" << "," << "\"" << "y" << "\"" << "," << "\"" << "Name of the trajectory" << "\"" << "," << "\"" << "Time" << "\""<< "\n";
				for (int i = 0; i < middleCurve.size(); i++){
					TrajectoryObs<double, T> point = middleCurve[i];
					filestreamOut << "\"" << point.pos[0] << "\"" << "," << "\"" << point.pos[1] << "\"" << "," << point.trajectoryName << "," << "\"" << point.time << "\"" << "\n";
				}
			}
			else{//T == 3
				filestreamOut << "\"" << "x" << "\"" << "," << "\"" << "y" << "\"" << "," << "\"" << "z" << "\"" << "," << "\"" << "index" << "\"" << "\n";
				for (int i = 0; i < middleCurve.size(); i++){
					TrajectoryObs<double, T> point = middleCurve[i];
					filestreamOut << "\"" << point.pos[0] << "\"" << "," << "\"" << point.pos[1] << "\"" << "," << "\"" << point.pos[2] << "\"" << "," << point.trajectoryName <<  "," << "\"" << point.time << "\"" << "\n";
				}
			}
			filestreamOut << "\"" << "epsilon" << "\"" << "\n" << "\"" << epsilon << "\"";
			cout << endl << "File write " << endl << endl;
            return 0;
		}
		catch (ios_base::failure& exc){
			cout << endl << "ERROR: directory not found" << endl;
            return 1;
		}
	}
};