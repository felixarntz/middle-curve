#pragma once

#include <math.h>
#include <vector>
#include <algorithm>
#include <bitset>
#include <stdlib.h>

#include "FS_Point.h"
#include "trajectory.h"
#include "base_algorithm.h"

/*! \brief Diskret-Frechet Algorithmus als basis für die den UnorderedCase Algorithmus
 *
 * Berechnet die Mittelkurve zwischen n Trajectory Objekten anhand des Diskret-Frechet Algorithmus.
 */

/**
* Autoren - Felix Arntz, Marcel Stepien, Dennis Pawlowski
*
* Datum - 05.07.2015
*/

using namespace std;

template<size_t T>
class DiscreteFrechet : public base_algorithm<T> {

protected:
	using base_algorithm<T>::calc_distance;
	using base_algorithm<T>::get_binary_choices;
	using base_algorithm<T>::coords_to_index;
	using base_algorithm<T>::index_to_coords;
	using base_algorithm<T>::add_coords;

	using base_algorithm<T>::m_dimension;
	using base_algorithm<T>::m_trajectories;
	using base_algorithm<T>::m_freespace_size;
	using base_algorithm<T>::m_shape_strides;

	vector<vector<int>> m_choices;
	vector<FS_Point<T>> m_result;
	vector<FS_Point<T>> m_freespace;

	int make_real_i(int i){
		return i;
	}

	int make_real_dimension(){
		return m_dimension;
	}

	virtual void fill_free_space() {

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
	/**
	 * Konstruktor der DiskretFrechet Klasse
	 *
	 * trajectories - ein Vector mit den Trajectory Objekten (siehe trajectory.h)
	 */
	DiscreteFrechet(vector<Trajectory<double, T>> trajectories) :base_algorithm<T>(trajectories){ }

	/**
	 * Diese funktion fuehrt die Berechnung aus und speichert das Ergebnis in den Attributen der Klasse.
	 */
	void run(){
		base_algorithm<T>::run();
		m_choices = get_binary_choices(1, (int)pow(2, m_dimension), m_dimension);
		fill_free_space();
		m_result = find_path();
	}

	/**
	 * Fuehrt die Suche nach dem existierenden Pfad durch. Wird von der Funktion run() automatisch aufgerufen.
	 *
	 * return - den Pfad als vector<FS_Point<T>> (siehe FS_Point.h)
	 */
	vector<FS_Point<T>> find_path() {
		FS_Point<T> * temp = find_cheapest_path(0);
		vector<FS_Point<T>> path;

		while (true){
			path.push_back(*temp);

			if (!temp->get_has_next()){
				break;
			}

			temp = temp->get_next();
		}

		return  path;
	}

	/**
	* Fuehrt die Suche nach dem existierenden günstigsten Pfad durch. 
	*
	* return - das anfangsobjekt als Pointer
	*/
	FS_Point<T> * find_cheapest_path(int index) {

		if (index == m_freespace_size - 1) {
			m_freespace[index].make_last();
		}
		else {
			if (m_freespace[index].get_visited()) {
				return &m_freespace[index];
			}
			else {
				double cheapest = 100000.0;
				FS_Point<T> * next = nullptr;
				for (auto choice : m_choices) {

					vector<int> next_coords = add_coords(index_to_coords(index), choice);

					int next_index = coords_to_index(next_coords);

					if (next_index > -1) {
						FS_Point<T> * current = find_cheapest_path(next_index);
						
						if (current->get_cost() < cheapest) {
							next = current;
							cheapest = current->get_cost();
						}
					}
				}
				m_freespace[index].set_next(next, cheapest);
			}
		}

		return &m_freespace[index];
	}

	/**
	 * return - 0
	 */
	int make_add_value(){ 
		return 0; 
	}

	/**
	 * Gibt das errechnete Ergebnis in der Konsole aus
	 */
	void printResults() {
		vector<int> i;

		for(auto it : m_result){

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

	/**
	 * return - das Ergebnis der Berechnung als vector<FS_Point<T>> Objekt (siehe FS_Point.h)
	 */
	vector<FS_Point<T>> getResult(){
		return m_result;
	}

	/**
	* return - die errechnete Mittelkurve vector
	*/
	vector<TrajectoryObs<double, T>> getMiddleCurve(){
		vector<TrajectoryObs<double, T>> temp;

		for (auto it : m_result){
			TrajectoryObs<double, T> obs = it.get_center_point();
			temp.push_back(obs);
		}

		return temp;
	}

	/**
	* return - der Epsilonwert der errechneten Mittelkurve
	*/
	double getEpsilon(){
		double epsilon = 0.0;

		for (auto it : m_result){
			if (epsilon < it.get_center_distance()){
				epsilon = it.get_center_distance();
			}
		}

		return epsilon;
	}
};