#pragma once
#include <stdio.h>
#include <math.h>

#include "base_algorithm.h"
#include "BS_Point.h"

using namespace std;

template<size_t T>
class OrderedCase : base_algorithm<T> {

protected:
	vector<BS_Point<T>> m_boolspace;
	int m_bool_size = 0;

	void fill_bool_space(){
		
		for (int i = 0; i < m_freespace_size; i++){
			vector<int> coords = index_to_coords(i);
			m_boolspace.push_back(BS_Point<T>(coords));
			
			vector<bool> temp;
			for (int x = 0; x < m_bool_size; x++){

				if (x == 0){
					//Rcontains neither P_i nor Q_j
					if (i == 0){
						temp.push_back(true);
					}
					else if (is_in_boundaries(coords, { 0, 0, 0, 0 }, { 0, 0, (int)(m_trajectories[0].size() - 1), (int)(m_trajectories[1].size() - 1)})){
						temp.push_back(false);
					}
					else if (is_in_boundaries(coords, { 0, 0, 0, 0 }, { (int)(m_trajectories[0].size() - 1), 0, (int)(m_trajectories[0].size() - 1), (int)(m_trajectories[1].size() - 1) })){
						temp.push_back(m_boolspace[coords_to_index(addadd_coords(coords, {-1, 0, 0, 0}))].getMainvalue());
					}
					else if (is_in_boundaries(coords, { 0, 0, 0, 0 }, { 0, (int)(m_trajectories[1].size() - 1), (int)(m_trajectories[0].size() - 1), (int)(m_trajectories[1].size() - 1) })){
						temp.push_back(m_boolspace[coords_to_index(addadd_coords(coords, { 0, -1, 0, 0 }))].getMainvalue());
					}
					else {
						temp.push_back(m_boolspace[coords_to_index(addadd_coords(coords, { -1, -1, 0, 0 }))].getMainvalue());
					}
					
				}
				else if (x == 1){

				}
				else if (x == 2){

				}
				else if (x == 3){

				}
				else if (x == 4){

				}
				else if (x == 5){

				}
				else if (x == 6){

				}
			}
		}
	}


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
		
		/*
		int i = 750;
		vector<int> temp = index_to_coords(i);

		vector<int> rahmen1{ 0, 0, 0, 0 };
		vector<int> rahmen2{ (int)(m_trajectories[0].size() - 1), (int)(m_trajectories[1].size() - 1), 0, 0 };

		if (base_algorithm::is_in_boundaries(temp, rahmen1, rahmen2)){
			cout << temp[0] << ", " << temp[1] << ", " << temp[2] << ", " << temp[3] << endl;
			cout << rahmen1[0] << ", " << rahmen1[1] << ", " << rahmen1[2] << ", " << rahmen1[3] << endl;
			cout << rahmen2[0] << ", " << rahmen2[1] << ", " << rahmen2[2] << ", " << rahmen2[3] << endl;
		}*/

		fill_bool_space();
	}

	void printResults(){
		
	}
};