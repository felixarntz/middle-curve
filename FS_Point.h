#pragma once

#include <vector>

#include "trajectory.h"
#include "base_point.h"


using namespace std;

/**
* Beschreibung: FS_Point definiert den Inhalt eines feldes im FreeSpace des Unordered-Case und des DiskretFrechet Algorithmus
*
* Autoren: Felix Arntz, Marcel Stepien, Dennis Pawlowski
*
* Datum: 05.07.2015
*
* Basierend auf einem Wissenschaftlichen Arbeit der Ruhr-Universität Bochum
*/

template<size_t T>
class FS_Point :public base_point<T>{

protected:
	double m_center_distance = 1000000.0;
	TrajectoryObs<double, T> m_center_point;

	double m_cost = 1000000.0;
	FS_Point<T> * m_next;
	//vector<FS_Point<T>> m_rest_path;  //B Speicherplatz

	bool m_has_next = false;
	bool m_visited = false;

public:
	using base_point<T>::get_dimension;
	using base_point<T>::get_indices;
	using base_point<T>::m_indices;

	FS_Point(vector<int> coords) : base_point<T>(coords){}

	void set_center_point(TrajectoryObs<double, T> point, double distance) {
		if (distance < m_center_distance){
			m_center_point = point;
			m_center_distance = distance;
		}
	}

	double get_center_distance() {
		return m_center_distance;
	}

	TrajectoryObs<double, T> get_center_point() {
		return m_center_point;
	}

	void set_next(FS_Point<T> * next, double cost) {
		if (m_center_distance + cost < m_cost) {
			m_next = next;
			m_cost = m_center_distance + cost;
			m_has_next = true;
		}
		m_visited = true;
	}

	double get_cost() {
		return m_cost;
	}

	FS_Point<T> * get_next() {
		return m_next;
	}


	bool get_has_next() {
		return m_has_next;
	}

	bool get_visited() {
		return m_visited;
	}

	void make_last(){
		m_cost = m_center_distance;
		m_next = nullptr;
		m_visited = true;
	}
};