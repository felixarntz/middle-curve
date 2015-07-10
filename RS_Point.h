#pragma once

#include <vector>
#include <algorithm>
#include <limits>

#include "trajectory.h"
#include "base_point.h"

using namespace std;

/**
* Beschreibung: RS_Point definiert den Inhalt eines feldes im FreeSpace des Restricted-Case Algorithmus
*
* Autoren: Felix Arntz, Marcel Stepien, Dennis Pawlowski
*
* Datum: 05.07.2015
*
* Basierend auf einem Wissenschaftlichen Arbeit der Ruhr-Universität Bochum
*/

template<size_t T>
class RS_Point :public base_point<T>{

protected:
	int m_dimension = 0;
	
	int previous = -1;

	int m_tra = -1;
	int m_traObs = -1;

	bool m_check = false;
	
public:
	using base_point<T>::get_dimension;
	using base_point<T>::get_indices;
	using base_point<T>::m_indices;


	RS_Point(vector<int> coords) : base_point<T>(coords){
		m_dimension = (int)(coords.size() / 2);
	}

	int get_previous(){
		return previous;
	}

	/*int get_centerIndex(){
		return centerIndex;
	}*/

	int get_tra(){
		return m_tra;
	}

	int get_traObs(){
		return m_traObs;
	}

	void set_previous(int point){
		previous = point;
	}

	void enable(int tra, int traObs){
		m_tra = tra;
		m_traObs = traObs;
		m_check = true;
	}

	int get_check(){
		return m_check;
	}
};