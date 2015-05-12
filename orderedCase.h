#pragma once
#include <stdio.h>
#include <math.h>

#include "base_algorithm.h"

using namespace std;

template<size_t T>
class OrderedCase : base_algorithm<T> {

protected:
	//vector<bool_point<T>> m_boolspace;
	int m_bool_size = 0;

	void fill_bool_space(){}

	int make_real_i(int i){
		if (i >= m_dimension){
			return i - m_dimension;
		}
		return i;
	}

	int make_real_dimension(){
		return m_dimension*2;
	}

public:
	OrderedCase(vector<Trajectory<double, T>> trajectories) : base_algorithm(trajectories){}

	void run(){
		base_algorithm::run();
		m_bool_size = pow(2, m_dimension - 1) * (m_dimension + 2) - 1;
	}

	void printResults(){
		for (int i = 0; i < m_shape_strides.size(); i++){
			cout << m_shape_strides[i] << " ";
		}
	}
};