#pragma once

#include <vector>
#include <algorithm>
#include <limits>

#include "trajectory.h"
#include "base_point.h"

using namespace std;

template<size_t T>
class BS_Point :public base_point<T>{

protected:
	int m_dimension = 0;
	double m_mainvalue = numeric_limits<float>::infinity();
	vector<double> m_values;

	int m_previous = -1;
	vector<int> m_previous_points;

	//TrajectoryObs<double, T>* center_point = nullptr;
	//vector<TrajectoryObs<double, T>*> center_points;

	int m_tra_index = -1;
	int m_traObs_index = -1;


	double m_cost = numeric_limits<float>::infinity();
	
public:
	using base_point<T>::get_dimension;
	using base_point<T>::get_indices;
	using base_point<T>::m_indices;

	BS_Point() : base_point<T>(vector<int>()){
		m_dimension = 0;
	}

	BS_Point(vector<int> coords) : base_point<T>(coords){
		m_dimension = (int)(coords.size() / 2);
	}
	
	int get_previous(){
		return m_previous;
	}

	int get_trajectory_index(){
		return m_tra_index;
	}
	
	int get_trajectoryObs_index(){
		return m_traObs_index;
	}

	void add_value(double value, int tra_index, int traObs_index , int point){	
		m_values.push_back(value);
		m_previous_points.push_back(point);
		
		if (value < m_mainvalue){
			m_mainvalue = value;
			m_tra_index = tra_index;
			m_traObs_index = traObs_index;
			m_previous = point;
		}
	}

	int get_dimension(){
		return m_dimension;
	}

	double get_mainvalue(){
		return m_mainvalue;
	}

	double get_cost() {
		return m_cost;
	}

	vector<double> get_values(){
		return m_values;
	}

	double get_value_at(int i){
		return m_values[i];
	}
};