#pragma once
#include <stdio.h>
#include <vector>

using namespace std;

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