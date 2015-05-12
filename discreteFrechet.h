#pragma once

#include <math.h>
#include <vector>
#include <algorithm>
#include <bitset>
#include <stdlib.h>

#include "FS_Point.h"
#include "trajectory.h"
#include "base_algorithm.h"

/*discreteFrechet Class Header*/

using namespace std;

template<size_t T>
class DiscreteFrechet : public base_algorithm<T> {

protected:
	
	vector<vector<int>> m_choices;
	vector<FS_Point<T>> m_result;
	vector<FS_Point<T>> m_freespace;
	
	int make_real_i(int i){
		return i;
	}

	int make_real_dimension(){
		return m_dimension;
	}

	void fill_free_space() {
		
		for (int i = 0; i < m_freespace_size; i++) {
			vector<int> coords = index_to_coords(i);
			m_freespace.push_back(FS_Point<T>(coords));
			double max_distance = 0.0;
			
			for (int j = 0; j < m_dimension; j++) {

				TrajectoryObs<double, T> current_point = m_trajectories[j][coords[j]];
				
				for (int k = 0; k < m_dimension; k++) {
					if (k != j) {
						TrajectoryObs<double, T> point = m_trajectories[k][coords[k]];
						double temp_Distance = calc_distance(point.pos, current_point.pos);
						if (temp_Distance > max_distance) {
							max_distance = temp_Distance;
						}
					}
				}
			}
			m_freespace[i].set_center_point(m_trajectories[0][coords[0]], max_distance);
		}
	}

public:
	DiscreteFrechet(vector<Trajectory<double, T>> trajectories):base_algorithm<T>(trajectories){ }

	void run(){	
		base_algorithm::run();
		m_choices = get_binary_choices();
		fill_free_space();
		m_result = find_path();
	}

	double find_epsilon(vector<FS_Point<T>> path) {
		double epsilon = 0.0;
		for(auto point : path) {
			double distance = point.get_center_distance();
			if (distance > epsilon) {
				epsilon = distance;
			}
		}
		return epsilon;
	}

	vector<FS_Point<T>> find_path() {
		return find_cheapest_path(0);
	}

	vector<FS_Point<T>> find_cheapest_path(int index) {
		vector<FS_Point<T>> path;
		vector<FS_Point<T>> rest_path;

		if (index == m_freespace_size - 1) {
			m_freespace[index].set_rest_path(vector<FS_Point<T>>(), 0.0);
			m_freespace[index].set_visited();
		}
		else {
			if (m_freespace[index].get_visited()) {
				rest_path = m_freespace[index].get_rest_path();
			}
			else {
				double cheapest = 100000.0;
				for(auto choice : m_choices) {
					
					vector<int> next_coords = add_coords(index_to_coords(index), choice);
					int next_index = coords_to_index(next_coords);
					
					if (next_index > -1) {
						vector<FS_Point<T>> current_path = find_cheapest_path(next_index);
						if (current_path[0].get_cost() < cheapest) {
							rest_path = current_path;
							cheapest = current_path[0].get_cost();
						}
					}
				}
				m_freespace[index].set_rest_path(rest_path, cheapest);
				m_freespace[index].set_visited();
			}
		}
		path.push_back(m_freespace[index]);

		for (auto it : rest_path){
			path.push_back(it);
		}

		return path;
	}

	double calc_distance(double pos1[], double pos2[]) {
		return sqrt(pow(pos1[0] - pos2[0], 2) + pow(pos1[1] - pos2[1], 2));
	}

	vector<int> add_coords(vector<int> coords1, vector<int> coords2) {
		vector<int> sum;
		for (int i = 0; i < m_dimension; i++) {
			sum.push_back(coords1[i] + coords2[i]);
		}
		return sum;
	}

	vector<vector<int>> get_binary_choices(){
		vector<vector<int>> choices;
	
		for (int i = 1; i < (int)pow(2, m_dimension); i++){
			string binary = bitset<32>(i).to_string();  //B
			binary = binary.substr(binary.length() - m_dimension, m_dimension);

			vector<int> choice;
			for (int i = 0; i < binary.length(); i++){
				choice.push_back(stoi(binary.substr(i, 1)));
			}
			
			choices.push_back(choice);
		}
		return choices;
	}

	//Ausgabe
	void printResults() {
		vector<int> i;
		
		for (auto it : m_result){
			
			cout << "(";
			vector<int> indi = it.get_indices();
			for (int i = 0; i < indi.size(); i++){
				cout << indi[i];
				if (i != indi.size() - 1){
					cout << ",";
				}
			}
			cout << ") | CenterPoint: (";
				
			
			for (int i = 0; i < (sizeof(it.get_center_point().pos) / sizeof(*it.get_center_point().pos)); i++){
				cout << it.get_center_point().pos[i];
				if (i != (sizeof(it.get_center_point().pos) / sizeof(*it.get_center_point().pos)) - 1){
					cout << ",";
				}
			}
			cout << ") | CenterDistance: " << it.get_center_distance() << endl;
		}
	}


};