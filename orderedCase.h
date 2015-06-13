#pragma once
#include <stdio.h>
#include <math.h>
#include <limits>
#include <algorithm>

#include "base_algorithm.h"
#include "BS_Point.h"

using namespace std;

template<size_t T>
class OrderedCase : base_algorithm<T> {

protected:
	vector<BS_Point<T>> m_boolspace;
	BS_Point<T> * m_result = nullptr;
	vector<vector<int>> m_choices;

	void fill_bool_space(){
		
		for (int i = 0; i < m_freespace_size; i++){
			vector<int> coords = index_to_coords(i);
			m_boolspace.push_back(BS_Point<T>(coords));
			vector<double> values;

			//Regel 1 (1 Boolvalue)
			values.push_back(rule_1(coords));

			//Regel 2 (2^k -2 Boolvalues)
			for (int j = 1; j <= (int)(pow(2, m_dimension) - 2); j++) {
				values.push_back(rule_2(coords, j));
			}

			//Regel 3 (k Boolvalues)
			for (int j = 1; j <= m_dimension; j++) {
				values.push_back(rule_3(coords, j));
			}

			//Regel 4 (k*(2^(k-1)-1) Boolvalues)
			for (int j = 1; j <= m_dimension; j++) {
				for (int k = 1; k <= (int)(pow(2, m_dimension - 1) - 1); k++){
					values.push_back(rule_4(coords, j, k));
				}
			}

			m_boolspace[i].set_values(values);
		}
	}

	double rule_1(vector<int> coords){
		//gib true, falls alle Koordinaten 0 sind
		if (coords_to_index(coords)==0){
			return 0.0;
		}

		bool all_zero = true;
		vector<int> choice;
		for (int i = 0; i < m_dimension; i++){
			if (coords[i] > 0){
				choice.push_back(1);
				all_zero = false;
			}
			else{
				choice.push_back(0);
			}
		}

		//gib false zurück falls alle oberen Koordinaten 0 sind
		if (all_zero){
			return numeric_limits<float>::infinity();
		}

		//Rück-Referenz auf Gesamtregel für alle vorderen Koordinaten > 0 um 1 verringert
		return m_boolspace[coords_to_index(add_coords(coords, get_add_coords(choice, 1)))].get_mainvalue();
	}

	double rule_2(vector<int> coords, int rule_counter){
		//Fehlerrückgabe: rule_counter beginnt immer bei 1
		if (rule_counter == 0){
			return numeric_limits<float>::infinity();
		}

		//gib false zurück, falls actuelle Koordinate 0, oder alle anderen vorderen Koordinaten 0
		vector<int> choice = make_Binary(rule_counter, m_dimension);
		bool others_zero = true;

		for (int i = 0; i < choice.size(); i++){
			if (choice[i] == 1 && coords[i] == 0){
				return numeric_limits<float>::infinity();
			}
			else if(choice[i] == 0 && coords[i] > 0){
				others_zero = false;
			}
		}

		if (others_zero){
			return numeric_limits<float>::infinity();
		}

		int added_index = coords_to_index(add_coords(coords, get_add_coords(choice, 2)));

		vector<double> values_to_compare;

		//rückreferenz auf regel 2 an der selben stelle
		values_to_compare.push_back(m_boolspace[added_index].get_value_at(get_rule_index(rule_counter, 2)));

		for (int i = 0; i < choice.size(); i++){
			if (choice[i] == 0){
				int subcounter = make_decimal(strip_choice_index(i, choice));

				if (subcounter == 0){ //TO-DO  Zugriff auf regel 3? Oder gar kein zugriff?

					values_to_compare.push_back(m_boolspace[added_index].get_value_at(get_rule_index(i+1, 3)));
				}
				else{
					values_to_compare.push_back(m_boolspace[added_index].get_value_at(get_rule_index(i + 1, 4, subcounter)));
				}

				/*if (subcounter > 0){ 
					values_to_compare.push_back(m_boolspace[added_index].get_value_at(get_rule_index(i + 1, 4, subcounter)));
				}*/

			}
		}

		//gib Minimalwert aus Rückreferenzen zurück
		return *min_element(values_to_compare.begin(), values_to_compare.end());
	}

	double rule_3(vector<int> coords, int rule_counter){
		// Fehlerrückgabe: rule_counter beginnt immer bei 1
		if (rule_counter == 0) {
			return numeric_limits<float>::infinity();
		}

		// gib false zurück, falls aktuelle Koordinate 0
		if (coords[rule_counter - 1] == 0) {
			return numeric_limits<float>::infinity();
		}

		// gib false zurück, falls eine der hinteren Koordinaten 0
		for (int i = m_dimension; i < m_dimension * 2; i++) {
			if (coords[i] == 0) {
				return numeric_limits<float>::infinity();
			}
		}

		vector<double> values_to_compare;

		// gib false zurück, falls einer der Distanzvergleiche mit Epsilon false ergibt
		TrajectoryObs<double, T> current_point = m_trajectories[rule_counter - 1][coords[rule_counter - 1]-1];

		for (int x = 0; x < m_dimension; x++) {
			TrajectoryObs<double, T> point = m_trajectories[x][coords[x + m_dimension] - 1];

			values_to_compare.push_back(calc_distance(point.pos, current_point.pos));
		}

		vector<double> subvalues_to_compare;

		// Rück-Referenz auf Regel 1
		subvalues_to_compare.push_back(m_boolspace[coords_to_index(add_coords(coords, get_add_coords(make_Binary(0, m_dimension), 3)))].get_value_at(get_rule_index(1, 1)));

		// Rück-Referenzen auf Regel 3 an derselben Stelle für alle Binärkombinationen > 0
		vector<vector<int>> choices = get_binary_choices(0, (int)pow(2, m_dimension) - 1, m_dimension);
		for(auto choice : choices) {
			subvalues_to_compare.push_back(m_boolspace[coords_to_index(add_coords(coords, get_add_coords(choice, 3)))].get_value_at(get_rule_index(rule_counter, 3)));
		}

		values_to_compare.push_back(*min_element(subvalues_to_compare.begin(), subvalues_to_compare.end()));

		// gib Maximalwert aus Rück-Referenzen zurück
		return *max_element(values_to_compare.begin(), values_to_compare.end());
	}
		

	double rule_4(vector<int> coords, int rule_counter, int inner_rule_counter){
		// Fehlerrückgabe: rule_counter beginnt immer bei 1
		if (rule_counter == 0) {
			return numeric_limits<float>::infinity();
		}

		// gib false zurück, falls aktuelle Koordinate 0
		if (coords[rule_counter - 1] == 0) {
			return numeric_limits<float>::infinity();
		}

		// gib false zurück, falls eine der hinteren Koordinaten 0
		for (int i = m_dimension; i < m_dimension * 2; i++) {
			if (coords[i] == 0) {
				return numeric_limits<float>::infinity();
			}
		}

		// gib false zurück, falls eine der vorderen Koordinaten 0 und die entsprechende hintere Koordinate 1
		for (int i = 0; i < m_dimension; i++) {
			if (coords[i] == 0 && coords[i + m_dimension] == 1) {
				return numeric_limits<float>::infinity();
			}
		}

		vector<double> values_to_compare;

		// gib false zurück, falls einer der Distanzvergleiche mit Epsilon false ergibt
		TrajectoryObs<double, T> current_point = m_trajectories[rule_counter - 1][coords[rule_counter - 1] - 1];

		for (int x = 0; x < m_dimension; x++) {
			TrajectoryObs<double, T> point = m_trajectories[x][coords[x + m_dimension] - 1];

			values_to_compare.push_back(calc_distance(point.pos, current_point.pos));
		}

		vector<double> subvalues_to_compare;

		int added_index = coords_to_index(add_coords(coords, get_add_coords(make_Binary(0, m_dimension), 4)));

		vector<int> choice = make_Binary(inner_rule_counter, m_dimension - 1);

		// Rück-Referenz auf Regel 2
		int rule_2_index = make_decimal(insert_choice_index(rule_counter - 1, choice));
		subvalues_to_compare.push_back(m_boolspace[added_index].get_value_at(get_rule_index(rule_2_index, 2)));

		// Rück-Referenzen auf Regel 3/4 für alle Stellen 1 der Binärzahl
		for (int i = 0; i < choice.size(); i++) {
			if (choice[i] == 1) {
				int subcounter = 0;
				if (rule_counter - 1 > i) {
					subcounter = make_decimal(strip_choice_index(i, insert_choice_index(rule_counter - 1, choice)));
				}
				else {
					subcounter = make_decimal(insert_choice_index(rule_counter - 1, strip_choice_index(i, choice)));
				}

				if (subcounter == 0) {
					subvalues_to_compare.push_back(m_boolspace[added_index].get_value_at(get_rule_index(i + 1, 3)));
				}
				else {
					subvalues_to_compare.push_back(m_boolspace[added_index].get_value_at(get_rule_index(i + 1, 4, subcounter)));
				}
			}
		}

		// Rück-Referenzen auf Regel 4 an derselben Stelle für jeweils eine der hinteren Stellen -1 gesetzt
		for (int i = 0; i < m_dimension; i++) {
			subvalues_to_compare.push_back(m_boolspace[coords_to_index(add_coords(coords, get_add_coords(make_Binary((int)pow(2, i), m_dimension), 4)))].get_value_at(get_rule_index(rule_counter, 4, inner_rule_counter)));
		}

		values_to_compare.push_back(*min_element(subvalues_to_compare.begin(), subvalues_to_compare.end()));

		// gib Maximalwert aus Rück-Referenzen zurück
		return *max_element(values_to_compare.begin(), values_to_compare.end());
	}

	BS_Point<T> * find_path() {
		vector<int> start_coords;
		for(int i = 0; i < m_dimension; i++) {
			start_coords.push_back(1);
		}
		for (int i = 0; i < m_dimension; i++) {
			start_coords.push_back(m_trajectories[i].size());
		}

		int start_index = coords_to_index(start_coords);
		return find_cheapest_path(start_index);
	}

	BS_Point<T> * find_cheapest_path(int index) {
 		if (index == m_freespace_size - 1) {
			m_boolspace[index].make_last();
		}
		else {
			if (m_boolspace[index].get_visited()) {
				return &m_boolspace[index];
			}
			else {
				double cheapest = 100000.0;
				BS_Point<T> * next = nullptr;
				for (auto choice : m_choices) {

					vector<int> next_coords = add_coords(index_to_coords(index), choice);

					int next_index = coords_to_index(next_coords);

					if (next_index > -1) {
						BS_Point<T> * current = find_cheapest_path(next_index);

						if (current->get_cost() < cheapest) {
							next = current;
							cheapest = current->get_cost();
						}
					}
				}
				m_boolspace[index].set_next(next, cheapest);
			}
		}

		return &m_boolspace[index];
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

	vector<int> insert_choice_index(int index, vector<int> choice){
		vector<int> new_choice;
		if (index == choice.size()){
			new_choice = choice;
			new_choice.push_back(0);
		}
		else{
			for (int i = 0; i < choice.size(); i++){
				if (i == index){
					new_choice.push_back(0);
				}
				new_choice.push_back(choice[i]);
			}
		}
		return new_choice;
	}

	vector<int> strip_choice_index(int index, vector<int> choice){
		vector<int> new_choice;
		
		for (int i = 0; i < choice.size(); i++){
			if (i != index){
				new_choice.push_back(choice[i]);
			}
		}

		return new_choice;
	}
	

	vector<int> get_add_coords(vector<int> binaryChoice, int rule){
		vector<int> output;

		switch (rule){
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

		return output;
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

	double calc_distance(double pos1[], double pos2[]) {
		return sqrt(pow(pos1[0] - pos2[0], 2) + pow(pos1[1] - pos2[1], 2));
	}

public:
	OrderedCase(vector<Trajectory<double, T>> trajectories) : base_algorithm(trajectories){}

	void run(){
		base_algorithm::run();
		m_choices = get_binary_choices(1, (int)pow(2, m_dimension), m_dimension);
		fill_bool_space();
		m_result = find_path();
	}

	void printResults(){
		vector<int> i;

		BS_Point<T> * temp = m_result;

		while (true){

			cout << "(";
			vector<int> indi = temp->get_indices();
			for (int i = 0; i < indi.size(); i++){
				cout << indi[i];
				if (i != indi.size() - 1){
					cout << ",";
				}
			}
		
			cout << ") | MainValue: " << temp->get_mainvalue() << endl;

			if (!temp->get_has_next()){
				break;
			}

			temp = temp->get_next();
		}
	}

};