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

	double m_epsilon = 5.0;


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
		//To Do
	}

	bool rule_3(vector<int> coords, int rule_counter){
		if (rule_counter < 1){
			return false;
		}

		if (coords[rule_counter-1] == 0 || has_coord_zero(coords, 0, m_dimension)){
			return false;
		}

		TrajectoryObs<double, T> current_point = m_trajectories[rule_counter - 1][coords[rule_counter - 1] - 1];
		for (int x = 0; x < m_dimension; x++){
			TrajectoryObs<double, T> temp_point = m_trajectories[x][coords[x + m_dimension] - 1];
			if (!check_distance(current_point.pos, temp_point.pos)){
				return false;
			}
		}

		vector<int> add_coords = get_add_coords(make_Binary(0, m_dimension), 3);
		if (m_boolspace[coords_to_index(add_coords(coords, add_coords))].getBoolvalueAt(get_rule_index(1, 1))){
			return true;
		}

		vector<vector<int>> choices = get_binary_choices(0, (int)(pow(2, m_dimension)-1), m_dimension);
		for (auto choice : choices){
			add_coords = get_add_coords(choice, 3);
			if (m_boolspace[coords_to_index(add_coords(coords, add_coords))].getBoolvalueAt(get_rule_index(rule_counter, 3))){
				return true;
			}
		}

		return false;
	}

	bool rule_4(vector<int> coords, int rule_counter, int inner_rule_counter){
		//To Do
	}

	int get_rule_index(int rule_counter, int rule = 0, int inner_rule_counter = 0){
		if (rule < 1 || rule > 4){
			return -1;
		}
		if (rule == 1){
			return 0;
		}

		rule_counter -= 1;
		int index = 1;

		if (rule > 2){
			index += pow(2, m_dimension)-2;
		}

		if (rule > 3){
			index += m_dimension + (rule_counter - 1)*(pow(2, m_dimension - 1) - 1);
			return index + inner_rule_counter;
		}

		return index + rule_counter;
	}

	int make_add_value(){
		return 1;
	}

	//Ueberprueft ob einer der Koordinaten in 'coords' eine 0 ist.
	bool has_coord_zero(vector<int> coords, int limit = 0, int offset = 0){
		if (limit == 0){
			limit = coords.size();
		}

		for (int i = offset; i < limit; i++){
			if (coords[i] == 0){
				return true;
			}
		}

		return false;
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

		case 3:
		case 4:
			for (int i = 0; i < m_dimension; i++){
				output.push_back(0);
			}
			for (int i = 0; i < m_dimension; i++){
				
				if (binaryChoice[i] == 0){
					output.push_back(-1);
				}
				else{
					output.push_back(0);
				}
				
			} break; 

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

	bool check_distance(double pos1[], double pos2[]){
		return (calc_distance(pos1, pos2) <= m_epsilon);
	}

	double calc_distance(double pos1[], double pos2[]) {
		return sqrt(pow(pos1[0] - pos2[0], 2) + pow(pos1[1] - pos2[1], 2));
	}

};