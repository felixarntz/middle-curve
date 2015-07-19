#pragma once

#include <vector>
#include <limits>

#include "trajectory.h"
#include "base_point.h"


using namespace std;

/*! \brief FS_Point definiert den Inhalt eines feldes im FreeSpace des Unordered-Case und des DiskretFrechet Algorithmus
*
* FS_Point ist ein Punktcontainer mit den errechneten Koordinaten des TrajectoryObs und ihrer Distanz zu den umliegenden Punkten.
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

template<size_t T>
class FS_Point :public base_point<T>{

protected:
	double m_center_distance = numeric_limits<double>::infinity();
	TrajectoryObs<double, T> m_center_point;

    double m_cost = numeric_limits<double>::infinity();
	int m_next = -1;

	bool m_has_next = false;
	bool m_visited = false;

public:
	using base_point<T>::get_dimension;
	using base_point<T>::get_indices;
	using base_point<T>::m_indices; 

	/**
	* Konstruktor des BS_Point Objekts
	*
	* coords - die Koordinaten des BS_Points im FreeSpace
	*/
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

	void set_next(int next, double cost) {
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

	int get_next() {
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
		
		m_visited = true;
	}
};