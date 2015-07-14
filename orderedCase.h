#pragma once
#include <stdio.h>
#include <math.h>
#include <limits>
#include <algorithm>

#include "base_algorithm.h"
#include "BS_Point.h"

using namespace std;

/*! \brief Ordered-Case Algorithmus zum berechen einer Middlecurve
*
* Berechnet die Mittelkurve zwischen Trajectorien anhand des OrderedCase Algorithmus
*
*/

/**
 *
 * Autoren: Felix Arntz, Marcel Stepien, Dennis Pawlowski
 *
 * Datum: 05.07.2015
 *
 * Basierend auf einer Wissenschaftlichen Arbeit der Ruhr-Universitaet Bochum
 */

template<size_t T>
class OrderedCase : base_algorithm<T> {

protected:
	//Funktionen und Variablen werden aus der Super-Klasse registriert
	using base_algorithm<T>::calc_distance;
	using base_algorithm<T>::get_binary_choices;
	using base_algorithm<T>::coords_to_index;
	using base_algorithm<T>::index_to_coords;
	using base_algorithm<T>::add_coords;

	using base_algorithm<T>::make_Binary;
	using base_algorithm<T>::make_decimal;

	using base_algorithm<T>::m_dimension;
	using base_algorithm<T>::m_trajectories;
	using base_algorithm<T>::m_freespace_size;
	using base_algorithm<T>::m_shape_strides;


	vector<BS_Point<T>> m_boolspace;
	vector<BS_Point<T>> m_result;
	vector<vector<int>> m_choices;

	BS_Point<T> current_bs_point;

	void fill_bool_space(){
		
		for (int i = 0; i < m_freespace_size; i++){
			
			vector<int> coords = index_to_coords(i);
			current_bs_point = BS_Point<T>(coords);

			//Zugriff auf Regel 1 (1 Boolvalue)
			rule_1(coords);


			//Zugriff auf Regel 2 (2^k -2 Boolvalues)
			for (int j = 1; j <= (int)(pow(2, m_dimension) - 2); j++) {
				rule_2(coords, j);
			}


			//Zugriff auf Regel 3 (k Boolvalues)
			for (int j = 1; j <= m_dimension; j++) {
				rule_3(coords, j);
			}

			
			//Zugriff auf Regel 4 (k*(2^(k-1)-1) Boolvalues)
			for (int j = 1; j <= m_dimension; j++) {
				for (int k = 1; k <= (int)(pow(2, m_dimension - 1) - 1); k++){
					rule_4(coords, j, k);
				}
			}

			m_boolspace.push_back(current_bs_point);
		}
	}

	void rule_1(vector<int> coords){
		//gib true, falls alle Koordinaten 0 sind
		if (coords_to_index(coords)==0){
			current_bs_point.add_value(0.0, -1, -1, -1);
		}
		else{

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
				current_bs_point.add_value(numeric_limits<float>::infinity(), -1, -1, -1);
			}
			else{
				int added_index = coords_to_index(add_coords(coords, get_add_coords(choice, 1)));
				double retval = m_boolspace[added_index].get_mainvalue();
				
				if (retval < numeric_limits<float>::infinity()){
					current_bs_point.add_value(retval, -1, -1, added_index);
				}
				else{
					current_bs_point.add_value(numeric_limits<float>::infinity(), -1, -1, -1);
				}
			}
		}
	}

	void rule_2(vector<int> coords, int rule_counter){
		//Fehlerrückgabe: rule_counter beginnt immer bei 1
		if (rule_counter == 0){
			current_bs_point.add_value(numeric_limits<float>::infinity(), -1, -1, -1);
		}
		else{
			//gib false zurück, falls actuelle Koordinate 0, oder alle anderen vorderen Koordinaten 0
			vector<int> choice = make_Binary(rule_counter, m_dimension);
			bool others = false;
			bool done = false;

			for (int i = 0; i < choice.size(); i++){
				if (choice[i] == 1 && coords[i] == 0){
					current_bs_point.add_value(numeric_limits<float>::infinity(), -1, -1, -1);
					done = true;
					break;
				}
				else if (choice[i] == 0 && coords[i] > 0){
					others = true;
				}
			}

			if (!done){
				if (!others){
					current_bs_point.add_value(numeric_limits<float>::infinity(), -1, -1, -1);
				}
				else{

					vector<int> add_coord = get_add_coords(choice, 2);
					int added_index = coords_to_index(add_coords(coords, add_coord));

					vector<double> values_to_compare;

					//rückreferenz auf regel 2 an der selben stelle
					values_to_compare.push_back(m_boolspace[added_index].get_value_at(get_rule_index(rule_counter, 2)));

					for (int i = 0; i < choice.size(); i++){
						if (choice[i] == 0){
							int subcounter = make_decimal(strip_choice_index(i, choice));
							values_to_compare.push_back(m_boolspace[added_index].get_value_at(get_rule_index(i + 1, 4, subcounter)));
						}
					}

					double retval = *min_element(values_to_compare.begin(), values_to_compare.end());
					if (retval < numeric_limits<float>::infinity()){

						current_bs_point.add_value(numeric_limits<float>::infinity(), -1, -1, added_index);
					}
					else{
						current_bs_point.add_value(numeric_limits<float>::infinity(), -1, -1, -1);
					}
				}
			}
		}
	}

	void rule_3(vector<int> coords, int rule_counter){
		// Fehlerrückgabe: rule_counter beginnt immer bei 1
		if (rule_counter == 0) {
			
			current_bs_point.add_value(numeric_limits<float>::infinity(), -1, -1, -1);
		}
		else{
			// gib false zurück, falls aktuelle Koordinate 0
			if (coords[rule_counter - 1] == 0) {
				
				current_bs_point.add_value(numeric_limits<float>::infinity(), -1, -1, -1);
			}
			else {

				// gib false zurück, falls eine der hinteren Koordinaten 0
				bool done = false;
				for (int i = m_dimension; i < m_dimension * 2; i++) {
					if (coords[i] == 0) {
						
						current_bs_point.add_value(numeric_limits<float>::infinity(), -1, -1, -1);
						done = true;
						break;
					}
				}

				if (!done){
					vector<double> values_to_compare;

					// gib false zurück, falls einer der Distanzvergleiche mit Epsilon false ergibt
					TrajectoryObs<double, T> current_point = m_trajectories[rule_counter - 1][coords[rule_counter - 1] - 1];

					for (int x = 0; x < m_dimension; x++) {
						TrajectoryObs<double, T> point = m_trajectories[x][coords[x + m_dimension] - 1];
						values_to_compare.push_back(calc_distance(point.pos, current_point.pos));
					}

					double min_val = numeric_limits<float>::infinity();
					int min_index = -1;

					vector<vector<int>> choices = get_binary_choices(0, (int)(pow(2, m_dimension) - 1), m_dimension);

					for (auto choice : choices){
						int added_index = coords_to_index(add_coords(coords, get_add_coords(choice, 3)));
						double retval = m_boolspace[added_index].get_value_at(get_rule_index(1, 1));

						if (retval < min_val){
							min_val = retval;
							min_index = added_index;
						}

						retval = m_boolspace[added_index].get_value_at(get_rule_index(rule_counter, 3));
						if (retval < min_val){
							min_val = retval;
							min_index = added_index;
						}
					}
					values_to_compare.push_back(min_val);

					double retval = *max_element(values_to_compare.begin(), values_to_compare.end());

					if (min_index >= 0){
						current_bs_point.add_value(retval, (rule_counter - 1), (coords[rule_counter - 1] - 1), min_index);
					}
					else{						
						current_bs_point.add_value(retval, (rule_counter - 1), (coords[rule_counter - 1] - 1), -1);
					}
				}
			}	
		}
	}

	void rule_4(vector<int> coords, int rule_counter, int inner_rule_counter){
		// Fehlerrückgabe: rule_counter beginnt immer bei 1
		if (rule_counter == 0) {
			current_bs_point.add_value(numeric_limits<float>::infinity(), -1, -1, -1);
		}
		else{

			// gib false zurück, falls aktuelle Koordinate 0
			if (coords[rule_counter - 1] == 0) {
				current_bs_point.add_value(numeric_limits<float>::infinity(), -1, -1, -1);
			}
			else{

				bool done = false;

				// gib false zurück, falls eine der hinteren Koordinaten 0
				for (int i = m_dimension; i < m_dimension * 2; i++) {
					if (coords[i] == 0) {
						current_bs_point.add_value(numeric_limits<float>::infinity(), -1, -1, -1);
						done = true;
						break;
					}
				}

				if (!done){
					// gib false zurück, falls eine der vorderen Koordinaten 0 und die entsprechende hintere Koordinate 1
					for (int i = 0; i < m_dimension; i++) {
						if (coords[i] == 0 && coords[i + m_dimension] == 1) {
							current_bs_point.add_value(numeric_limits<float>::infinity(), -1, -1, -1);
							done = true;
							break;
						}
					}

					if (!done){
						vector<double> values_to_compare;

						// gib false zurück, falls einer der Distanzvergleiche mit Epsilon false ergibt
						TrajectoryObs<double, T> current_point = m_trajectories[rule_counter - 1][coords[rule_counter - 1] - 1];
			
						for (int x = 0; x < m_dimension; x++) {
							TrajectoryObs<double, T> point = m_trajectories[x][coords[x + m_dimension] - 1];

							values_to_compare.push_back(calc_distance(point.pos, current_point.pos));
						}

						
						double min_val = numeric_limits<float>::infinity();
						int min_index = -1;


						vector<int> inner_rule_counter_binary = make_Binary(inner_rule_counter, m_dimension - 1);

						vector<vector<int>> choices = get_binary_choices(0, (int)(pow(2, m_dimension) - 1), m_dimension);


						for (auto choice : choices){
							int added_index = coords_to_index(add_coords(coords, get_add_coords(choice, 4)));

							//Rückferenz auf regel 2
							int rule_2_index = make_decimal(insert_choice_index(rule_counter - 1, inner_rule_counter_binary));
							double retval = m_boolspace[added_index].get_value_at(get_rule_index(rule_2_index, 2));

							if (retval < min_val){
								min_val = retval;
								min_index = added_index;
							}

							// Rück-Referenzen auf Regel 3/4 für alle Stellen 1 der Binärzahl
							for (int a = 0; a < inner_rule_counter_binary.size(); a++) {
								if (inner_rule_counter_binary[a] == 1) {
									int subcounter = 0;
									if (rule_counter - 1 > a) {
										subcounter = make_decimal(strip_choice_index(a, insert_choice_index(rule_counter - 1, inner_rule_counter_binary)));
									}
									else {
										subcounter = make_decimal(insert_choice_index(rule_counter - 1, strip_choice_index(a, inner_rule_counter_binary)));
									}
									if (subcounter == 0) {
										retval = m_boolspace[added_index].get_value_at(get_rule_index(a + 1, 3));
									}
									else {
										retval = m_boolspace[added_index].get_value_at(get_rule_index(a + 1, 4, subcounter));
									}
									if (retval < min_val) {
										min_val = retval;
										min_index = added_index;
									}
								}
							}
							
						}


						for (int i = 0; i < m_dimension; i++) {
							int added_index = coords_to_index(add_coords(coords, get_add_coords(make_Binary((int)(pow(2, i)), m_dimension), 4)));
							double retval = m_boolspace[added_index].get_value_at(get_rule_index(rule_counter, 4, inner_rule_counter));
							
							if (retval < min_val) {
								min_val = retval;
								min_index = added_index;
							}
						}
						values_to_compare.push_back(min_val);

						
						// gib Maximalwert aus Rück-Referenzen zurück
						double retval = *max_element(values_to_compare.begin(), values_to_compare.end());
						if (min_index >= 0){
							current_bs_point.add_value(retval, (rule_counter - 1), (coords[rule_counter - 1]-1), min_index);
						}
						else{
							current_bs_point.add_value(retval, (rule_counter - 1), (coords[rule_counter - 1]-1), -1);
						}

						
					}
				}

				
			}
		}
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

	vector<BS_Point<T>> find_path() {
		vector<BS_Point<T>> path;

		vector<int> start_coord;
		for (int i = 0; i < m_dimension; i++){
			start_coord.push_back(m_trajectories[i].size());
		}
		for (int i = 0; i < m_dimension; i++){
			start_coord.push_back(m_trajectories[i].size());
		}
		int index = coords_to_index(start_coord);

		BS_Point<T> temp = m_boolspace[index];

		while (true){
			path.push_back(temp);

			if (!(temp.get_previous() > -1)){
				//cout << (*temp).get_mainvalue() << endl;
				break;
			}

			temp = m_boolspace[temp.get_previous()];
		}

		reverse(path.begin(), path.end());

		return path;
	}

public:
	/**
	* Konstruktor der OrderedCase Klasse
	*
	* trajectories - ein Vector mit den Trajectory Objekten (siehe trajectory.h)
	*/
	OrderedCase<T>(vector<Trajectory<double, T>> trajectories) : base_algorithm<T>(trajectories){}

	/**
	* Diese funktion fuehrt die Berechnung aus und speichert das Ergebnis in den Attributen der Klasse.
	*/
	void run(){
		base_algorithm<T>::run();
		m_choices = get_binary_choices(1, (int)pow(2, m_dimension), m_dimension);
		fill_bool_space();
		m_result = find_path();
	}

	/**
	* Gibt das errechnete Ergebnis in der Konsole aus
	*/
	void printResults() {
		vector<int> i;

		for (auto it : m_result){
			if (it.get_trajectory_index() != -1 && it.get_trajectoryObs_index() != -1){

				int tra_ind = it.get_trajectory_index();
				int traObs_ind = it.get_trajectoryObs_index();

				TrajectoryObs<double, T> obs = m_trajectories[tra_ind][traObs_ind];
				int obs_size = (sizeof(obs.pos) / sizeof(*obs.pos));

				cout << "(";
				vector<int> indi = it.get_indices();
				for (int i = 0; i < indi.size(); i++){
					cout << indi[i];
					if (i != indi.size() - 1){
						cout << ",";
					}
				}

				cout << ") | CenterPoint: (";

				
				for (int i = 0; i < obs_size; i++){
					cout << obs.pos[i];
					if (i != obs_size - 1){
						cout << ",";
					}
				}
				cout << ") | CenterDistance: " << it.get_mainvalue() << endl;
			}

		}
	}

	/**
	* return - das Ergebnis der Berechnung als vector<FS_Point<T>> Objekt (siehe FS_Point.h)
	*/
	vector<BS_Point<T>> getResult(){
		return m_result;
	}

	/**
	* return - die errechnete Mittelkurve als vector<TrajectoryObs<double, SIZE>>
	*/
	vector<TrajectoryObs<double, T>> getMiddleCurve(){
		vector<TrajectoryObs<double, T>> temp;

		for (auto it : m_result){
			if (it.get_trajectory_index() != -1 && it.get_trajectoryObs_index() != -1){

				TrajectoryObs<double, T> obs = m_trajectories[it.get_trajectory_index()][it.get_trajectoryObs_index()];
				temp.push_back(obs);
			}

		}
		return temp;
	}

	/**
	* return - der Epsilonwert der errechneten Mittelkurve
	*/
	double getEpsilon(){
		return m_result[m_result.size()-1].get_mainvalue();
	}
};