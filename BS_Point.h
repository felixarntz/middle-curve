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
	double m_mainvalue = false;
	vector<double> m_values;

	BS_Point<T> * m_next = nullptr;
	double m_cost = numeric_limits<float>::infinity();
	bool m_visited = false;
	bool m_has_next = false;

public:
	using base_point<T>::get_dimension;
	using base_point<T>::get_indices;
	using base_point<T>::m_indices;


	BS_Point(vector<int> coords) : base_point<T>(coords){
		m_dimension = (int)(coords.size() / 2);
	}

	void set_values(vector<double> val){
		m_values = val;
		m_mainvalue = *min_element(val.begin(), val.end());
	}

	BS_Point<T>* get_next(){
		return m_next;
	}

	void set_next(BS_Point<T> * point, double cost){
		if (m_mainvalue+cost < m_cost){
			m_next = point;
			m_cost = m_mainvalue + cost;
			m_has_next = true;
		}
		m_visited = true;
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

	bool get_visited(){
		return m_visited;
	}

	void make_last(){
		m_cost = m_mainvalue;
		m_next = nullptr;
		m_visited = true;
	}

	bool get_has_next() {
		return m_has_next;
	}
};