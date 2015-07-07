#pragma once
#include <stdio.h>
#include <vector>
#include <limits>

using namespace std;

/**
* Beschreibung: Abstracte Basis-klasse f�r die Punktobjecte des FreeSpace
*
* Autoren: Felix Arntz, Marcel Stepien, Dennis Pawlowski
*
* Datum: 05.07.2015
*
* Basierend auf einem Wissenschaftlichen Arbeit der Ruhr-Universit�t Bochum
*/

template<size_t t>
class base_point{
protected:
	vector<int> m_indices;

public:
	base_point(vector<int> coords){
		m_indices = coords;
	}

	vector<int> get_indices() {
		return m_indices;
	}

	int get_dimension() {
		return m_indices.size();
	}
};