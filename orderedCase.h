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

	void fill_bool_space(){
		
		for (int i = 0; i < m_freespace_size; i++){
			vector<int> coords = index_to_coords(i);
			m_boolspace.push_back(BS_Point<T>(coords));
			vector<bool> bool_values;

			//Regel 1 (1 Boolvalue)
			bool_values.push_back(rule_1(coords));

			//Regel 2 (2^k -2 Boolvalues)
			for (int j = 1; j <= (int)(pow(2, m_dimension) - 2); j++) {
				bool_values.push_back(rule_2(coords, j));
			}

			//Regel 3 (k Boolvalues)
			for (int j = 1; j <= m_dimension; j++) {
				bool_values.push_back(rule_3(coords, j));
			}

			//Regel 4 (k*(2^(k-1)-1) Boolvalues)
			for (int j = 1; j <= m_dimension; j++) {
				for (int k = 1; k <= (int)(pow(2, m_dimension - 1) - 1); k++){
					bool_values.push_back(rule_4(coords, j, k));
				}
			}

			m_boolspace[i].set_boolvalues(bool_values);
		}
	}

	bool rule_1(vector<int> coords){
		if (coords_to_index(coords) == 0){
			return true;
		}
		
		vector<int> choice = make_Binary(0, m_dimension);
		vector<int> boundary_Coords[2] = get_Boundary_Coords(choice, 1);
		if (is_in_boundaries(coords, boundary_Coords[0], boundary_Coords[1])){
			return false;
		}
		
		vector<vector<int>> choices = get_binary_choices(1, (int)pow(2, m_dimension), m_dimension);
		for (choice : choices){
			boundary_Coords = get_Boundary_Coords(choice, 1);
			if (is_in_boundaries(coords, boundary_Coords[0], boundary_Coords[1])){
		
				vector<int> temp_add_coords = get_add_coords(choice, 1);
				return m_boolspace[coords_to_index(add_coords(coords, temp_add_coords))].getMainvalue();
			
			}
		}

		return false;
	}

	bool rule_2(vector<int> coords, int rule_counter){

	}

	bool rule_3(vector<int> coords, int rule_counter){

	}

	bool rule_4(vector<int> coords, int rule_counter, int inner_rule_counter){

	}

	vector<int> get_Boundary_Coords(vector<int> binaryChoice, int rule){
		vector<int> output[2] = {
			vector<int>,
			vector<int>
		};
		//fill(output.begin(), output.end(), vector<int>(2*m_dimension));

		switch (rule){
		case 1: 
		case 2: 
			for (int i = 0; i < m_dimension; i++){
				if (binaryChoice[i] == 1){
					output[1].push_back(m_trajectories[i].size() - 1);
				}
				else{
					output[1].push_back(0);
				}
				output[0].push_back(0);
			} 
			
			for (int i = 0; i < m_dimension; i++){
				output[1].push_back(m_trajectories[i].size() - 1);	
				output[0].push_back(0);
			} break;

		case 3: break; //B
		case 4: break; //B

		default: 
			for (int i = 0; i < (2*m_dimension); i++){
				output[1].push_back(m_trajectories[i].size() - 1);
				output[0].push_back(0);
			} break;
		}

		return output;
	}

	vector<int> get_add_coords(vector<int> binaryChoice, int rule){
		vector<int> output;

		switch (){
		case 1: 
		case 2: 
			for (int i = 0; i < m_dimension; i++){
				if (binaryChoice[i] == 1){
					output.push_back(-1);
				}
				else{
					output.push_back(0);
				}
			} 

			for (int i = 0; i < m_dimension; i++){
				output.push_back(0);
			} break;

		case 3: break; //B
		case 4: break; //B

		default:
			for (int i = 0; i < (2*m_dimension); i++){
				output.push_back(0);
			} break;
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