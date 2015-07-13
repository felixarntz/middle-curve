#pragma once
#include <stdio.h>
#include <vector>
#include <limits>

using namespace std;

/*! \brief Basis-klasse fuer die Punktobjekte des FreeSpace
 * 
 * Diese Klasse enhaelt funktionen und Attribute die von allen Punktobjekten benoetigt werden.
 *
 */

/**
*
* Autoren: Felix Arntz, Marcel Stepien, Dennis Pawlowski
*
* Datum: 05.07.2015
*
* Basierend auf einer Wissenschaftlichen Arbeit der Ruhr-Universitaet Bochum
*/

template<size_t t>
class base_point{
protected:
	vector<int> m_indices;

public:
	/**
	 * Konstruktor der base_point Klasse
	 *
	 * coords - ein vector mit den Koordinate als integer Werte
	 */
	base_point(vector<int> coords){
		m_indices = coords;
	}

	/**
	 * return - gibt die Koordinaten des Punktes als vector<int> zurueck
	 */
	vector<int> get_indices() {
		return m_indices;
	}

	/**
	 * return - gibt die Dimension des Punktes als Integer zurueck
	 */
	int get_dimension() {
		return m_indices.size();
	}
};