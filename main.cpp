// Compile with "g++ -std=c++11 main.cpp"


#include <vector>
#include <iostream>
#include "trajectory.h"
#include "discreteFrechet.h"
#include "unorderedCase.h"
#include "orderedCase.h"
#include "CSV_IO.h"
#include "restrictedCase.h"

int main (int argc, char** argv) {
    
    /*Dies ist eine beispielhafte Ausführung
     fuer eine erzeugung einer "middle Curve"*/
    
    
    //delim -> Trennungssymbol der Daten in der CSV-Datei
	char delim = ',';
    
    
    /*Objekterzeugung fuer das einlesen der Daten aus einer CSV Datei
     Wahlweise fuer 2 oder 3 dimensionale Trajektorien*/
	CSV_IO<2> reader;
	//CSV_IO<3> reader;
    
    
	/*Einlesen einer oder mehrerer Dateien*/
	reader.read("<Pfad>\\datei_1.csv", delim);
	reader.read("<Pfad>\\datei_2.csv", delim);


    
    
    /*Auswahl des Algorithmus fuer 2-dimensionale Trajektorien*/
	DiscreteFrechet<2> un(reader.getData());
	//UnorderedCase<2> un(reader.getData());
	//OrderedCase<2> un(reader.getData());
	//RestrictedCase<2> un(reader.getData());
    
    
    /*Auswahl des Algorithmus fuer 3-dimensionale Trajektorien*/
    //DiscreteFrechet<3> un(reader.getData());
    //UnorderedCase<3> un(reader.getData());
    //OrderedCase<3> un(reader.getData());
    //RestrictedCase<3> un(reader.getData());
    
    /*Der Algorithmus wird gestartet*/
	un.run();
	
    
    /*Ergebnisse werden auf der Konsole ausgegeben*/
	un.printResults();

    /*Pfad -> Speicherot der Datei -> Dateiname lautet immer middleCurve.csv (bitte dies nicht in der Variable
     pfad mitangeben)*/
	string pfad = "C:\\<Pfad>\\";
    
	double epsilon = un.getEpsilon();
    
    /*middle curve wird ausgegeben und in eine CSV-Datei gespeichert*/
    vector<TrajectoryObs<double, 2>> middle_curve = un.getMiddleCurve();
	reader.write(middle_curve, epsilon, pfad);

	return 0;
}
