#pragma once

#include <vector>
#include <algorithm>
#include <limits>

#include "trajectory.h"
#include "base_point.h"

using namespace std;

template<size_t T>
class RS_Point :public base_point<T>{

protected:
	int m_dimension = 0;
	
	RS_Point<T> * m_befor = nullptr;

	int centerIndex = -1;

	bool m_check = false;
	
public:
	using base_point<T>::get_dimension;
	using base_point<T>::get_indices;
	using base_point<T>::m_indices;


	RS_Point(vector<int> coords) : base_point<T>(coords){
		m_dimension = (int)(coords.size() / 2);
	}

	RS_Point<T> * get_befor(){
		return m_befor;
	}

	int get_centerIndex(){
		return centerIndex;
	}

	void set_befor(RS_Point<T> * point){
		m_befor = point;
	}

	void set_center(int center){
		centerIndex = center;
		m_check = true;
	}

	int get_check(){
		return m_check;
	}
};