#pragma once

#include <vector>

#include "trajectory.h"
#include "base_point.h"


using namespace std;

template<size_t T>
class FS_Point:public base_point<T>{

protected:
	
	double m_center_distance = 1000000.0;
	TrajectoryObs<double, T> m_center_point;
	
	double m_cost = 1000000.0;
	vector<FS_Point<T>> m_rest_path;  //B Speicherplatz
	bool m_visited = false;

public:
	FS_Point(vector<int> coords):base_point<T>(coords){}

	

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

	void set_rest_path(vector<FS_Point<T>> rest_path, double cost) {
		if (m_center_distance + cost < m_cost) {
			m_rest_path = rest_path;
			m_cost = m_center_distance + cost;
		}
	}

	double get_cost() {
		return m_cost;
	}

	vector<FS_Point<T>> get_rest_path() {
		return m_rest_path;
	}

	void set_visited() {
		m_visited = true;
	}

	bool get_visited() {
		return m_visited;
	}

	
};