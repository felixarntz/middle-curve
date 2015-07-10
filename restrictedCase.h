#pragma once
#include "base_algorithm.h"
#include "RS_Point.h"

#include <map>

/**
* Beschreibung: Restricted-Case Algorithmus zum berechen einer Middlecurve
*
* Autoren: Felix Arntz, Marcel Stepien, Dennis Pawlowski
*
* Datum: 05.07.2015
*
* Basierend auf einem Wissenschaftlichen Arbeit der Ruhr-Universität Bochum
*/

template<size_t T>
class RestrictedCase : public base_algorithm<T>{
	
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


	//Attribute
	vector<RS_Point<T>> m_space;
	vector<int> xboundaries;
	int xboundaries_size = 0;
	vector<int> xshape_strides;
	vector<RS_Point<T>> m_result;

	double m_epsilon = 0.0;

	void fill_space(){
		
		xboundaries.erase(xboundaries.begin(), xboundaries.end());
		m_space.erase(m_space.begin(), m_space.end());
		
		for (int i = 0; i < m_freespace_size; i++){
			vector<int> coords = index_to_coords(i);
			m_space.push_back(RS_Point<T>(coords));
			
			if (i == 0){
				//Erster Value wird true gesetzt
				m_space[i].enable(-1, -1);
			}
		}
		

		for (int i = 0; i < xboundaries_size; i++){
			if (i == 0){
				xboundaries.push_back(0);
			}
			else{
				xboundaries.push_back(-1);
			}
		}

		
		for (int i = 0; i < m_freespace_size; i++){

			vector<int> coords = index_to_coords(i);
			bool has_coords_zero = false;

			
			for (int j = 0; j < m_dimension; j++){
				if (coords[j] < 1){
					has_coords_zero = true;
					break;
				}
			}
			if (!has_coords_zero){
				for (int d = 0; d < m_dimension; d++){
					TrajectoryObs<double, T> current_point = m_trajectories[d][coords[d] - 1];
					if (check_distance(current_point, coords)){
						if (!m_space[i].get_check()){
							
							//vector<vector<int>> lower_left_wedge = get_lower_left_wedge(current_point, coords);
							//vector<vector<int>> extended_lower_left_wedge = make_extended_lower_left_wedge(lower_left_wedge, coords);
					
							vector<vector<int>> extended_lower_left_wedge = get_extended_lower_left_wedge(current_point, coords);


							int inter = intersects(extended_lower_left_wedge);

							if (inter > -1){
								vector<vector<int>> tempCoords;
								tempCoords.push_back(coords);
								add_wedge(coords, d, tempCoords, inter);
							}
						}
						if (m_space[i].get_check()){
							
							vector<vector<int>> upper_right_wedge = get_upper_right_wedge(current_point, coords);
							add_wedge(coords, d, upper_right_wedge, i);
							
						}

					}
				}
			}
		}
	}

	int make_real_i(int i){
		return i;
	}

	int make_real_dimension(){
		return m_dimension;
	}

	int make_add_value(){
		return 1;
	}

	vector<vector<int>> get_extended_lower_left_wedge(TrajectoryObs<double, T> current_point, vector<int> coords){
		vector<vector<int>> wedge;
		vector<int> min_coords = coords;


		for (int d = 0; d < m_dimension; d++){
			for (int i = coords[d]; i > 0; i--){
				if (calc_distance(current_point.pos, m_trajectories[d][i-1].pos) > m_epsilon){
					break;
				}
				min_coords[d] = i;
			}
		}

		for (int i = coords_to_index(coords); i >= 0; i--){
			vector<int> wedge_coords = index_to_coords(i);
			bool add = true;

			for (int j = 0; j < m_dimension; j++){
				if (wedge_coords[j] > coords[j]){
					add = false;
					break;
				}

				if (wedge_coords[j] < (min_coords[j] - 1)){
					add = false;
					break;
				}
			}

			if (add){
				wedge.push_back(wedge_coords);
			}
		}
		return wedge;
	}

	vector<vector<int>> get_upper_right_wedge(TrajectoryObs<double, T> current_point, vector<int> coords){
		vector<vector<int>> wedge;
		vector<int> max_coords = coords;


		for (int d = 0; d < m_dimension; d++){
			
			for (int i = coords[d]; i <= m_trajectories[d].size(); i++){
				if (i < 1){
					break;
				}

				if (calc_distance(current_point.pos, m_trajectories[d][i - 1].pos) > m_epsilon){
					break;
				}
				max_coords[d] = i;
			}
		}

		/*
		for (int i = coords_to_index(coords); i >= 0; i--){
		*/
		//for (int i = coords_to_index(max_coords); i >= coords_to_index(coords); i--){
		for (int i = coords_to_index(coords); i < m_freespace_size; i++){
			vector<int> wedge_coords = index_to_coords(i);
			bool add = true;

			for (int j = 0; j < m_dimension; j++){
				if (wedge_coords[j] < coords[j]){
					add = false;
					break;
				}

				if (wedge_coords[j] > max_coords[j]){
					add = false;
					break;
				}
			}

			if (add){
				wedge.push_back(wedge_coords);
			}
		}
		return wedge;
	}

	void add_wedge(vector<int> coords, int trajectory_index, vector<vector<int>> wedge, int previous_index){
		int max_last_coord = 0;

		
		for (int i = 0; i < wedge.size(); i++){
			vector<int> wedge_xcoords = coords_to_xcoords(wedge[i]);
			if (wedge[i][m_dimension - 1] > xboundaries[xcoords_to_index(wedge_xcoords)]){
				int wedge_index = coords_to_index(wedge[i]);

				if (wedge_index != previous_index){
					TrajectoryObs<double, T> current_point = m_trajectories[trajectory_index][coords[trajectory_index]-1];

					m_space[wedge_index].enable(trajectory_index, coords[trajectory_index]-1);
					m_space[wedge_index].set_previous(previous_index);

					if (wedge[i][m_dimension-1] > max_last_coord){
						max_last_coord = wedge[i][m_dimension - 1];
					}

				}
			}
			int xindex = xcoords_to_index(wedge_xcoords);
			if (max_last_coord > xboundaries[xindex]){
				xboundaries[xindex] = max_last_coord;
			}
		}
	}

	int intersects(vector<vector<int>> wedge){
		
		for (int i = 0; i < wedge.size(); i++){
			vector<int> wedge_xcoords = coords_to_xcoords(wedge[i]);
			int wedge_xindex = xcoords_to_index(wedge_xcoords);
			int last_coord = wedge[i][m_dimension - 1];

			if (last_coord <= xboundaries[wedge_xindex]){
				wedge_xcoords.push_back(xboundaries[wedge_xindex]);
				return coords_to_index(wedge_xcoords);
			}
		}
		return -1;
	}

	bool check_distance(TrajectoryObs<double, T> current_point, vector<int> coords){
		
		for (int d = 0; d < coords.size(); d++){
			TrajectoryObs<double, T> point = m_trajectories[d][coords[d] - 1];

			if (calc_distance(current_point.pos, point.pos) > m_epsilon){
				return false;
			}
		}

		return true;
	}

	vector<int> coords_to_xcoords(vector<int> coords){
		vector<int> temp;
		for (int i = 0; i < m_dimension - 1; i++){
			temp.push_back(coords[i]);
		}
		return temp;
	}

	int xcoords_to_index(vector<int> coords) {
		if (coords.size() != m_dimension-1) {
			return -1;
		}

		int add_value = make_add_value();

		int index = 0;
		for (int i = m_dimension - 2; i >= 0; i--) {
			if (coords[i] >= m_trajectories[i].size() + add_value) {
				return -1;
			}
			index += coords[i] * xshape_strides[i];
		}

		return index;
	}

	vector<int> index_to_xcoords(int index) {
		vector<int> coords;
		for (int i = 0; i < m_dimension-1; i++) {
			coords.push_back(-1);
		}

		if (index >= xboundaries_size) {
			return coords;
		}

		for (int i = m_dimension - 2; i >= 0; i--) {
			coords[i] = (int)floor(index / xshape_strides[i]);
			index = index % xshape_strides[i];
		}
		return coords;
	}

	vector<double> get_possible_epsilon(){
		map<double, int> epList;

		for (int a = 0; a < m_trajectories.size(); a++){
			for (int b = 0; b < m_trajectories.size(); b++){
				for (int i = 0; i < m_trajectories[a].size(); i++){
					for (int j = 0; j < m_trajectories[b].size(); j++){
						if (a != b || i != j){
							double temp_Distance = calc_distance(m_trajectories[a][i].pos, m_trajectories[b][j].pos);
							epList.insert(pair<double, int>(temp_Distance, 0));
						}
					}
				}
			}
		}

		vector<double> keys;
		
		for (map<double, int>::iterator it = epList.begin(); it != epList.end(); ++it) {
			keys.push_back(it->first);
		}
		
		return keys;
	}


	vector<RS_Point<T>> find_path() {
		vector<RS_Point<T>> path;

		vector<int> start_coord;

		for (int i = 0; i < m_dimension; i++){
			start_coord.push_back(m_trajectories[i].size());
		}

		int index = coords_to_index(start_coord);

		/*RS_Point<T> temp = m_space[index];
		while (true){
			path.push_back(temp);

			if (!(temp.get_previous() > -1)){
				//cout << (*temp).get_mainvalue() << endl;
				break;
			}

			temp = m_space[temp.get_previous()];
		}*/

		while (index > -1){
			RS_Point<T> current = m_space[index];
			path.push_back(current);
			index = current.get_previous();
		}

		reverse(path.begin(), path.end());

		return path;
	}


public:
	//Konstruktor
	RestrictedCase(vector<Trajectory<double, T>> trajectories) : base_algorithm<T>(trajectories){ }

	//Methoden
	void run(){
		base_algorithm<T>::run();
		
		int add_value = make_add_value();

		xshape_strides.push_back(1);
		xboundaries_size = m_trajectories[0].size() + add_value;
		
		for (int i = 1; i < m_dimension-1; i++) {
			xshape_strides.push_back(xshape_strides[i - 1] * (m_trajectories[modulo(i - 1, m_dimension-1)].size() + add_value));
			xboundaries_size *= (m_trajectories[i].size() + add_value);
		}


		vector<double> epsilons = get_possible_epsilon();

		for (int i = 0; i < epsilons.size(); i++){
			m_epsilon = epsilons[i];
			
			fill_space();

			vector<int> last_coords;

			for (int j = 0; j < m_dimension; j++){
				last_coords.push_back(m_trajectories[j].size());
			}

			if (m_space[coords_to_index(last_coords)].get_check()){
				break;
			}
		}

		m_result = find_path();
	}

	vector<TrajectoryObs<double, T>> getMiddleCurve(){
		vector<TrajectoryObs<double, T>> temp;

		for (auto it : m_result){
			int tra = it.get_tra();
			int traObs = it.get_traObs();

			if (tra != -1 && traObs != -1){

				TrajectoryObs<double, T> obs = m_trajectories[tra][traObs];
				temp.push_back(obs);
			}

		}
		return temp;
	}

	double getEpsilon(){
		return m_epsilon;
	}

	/**
	*Ausgabe in der Console
	*/
	void printResults() {
		vector<int> i;

		for (auto it : m_result){

			int tra = it.get_tra();
			int traObs = it.get_traObs();

			cout << "(";
			vector<int> indi = it.get_indices();
			for (int i = 0; i < indi.size(); i++){
				cout << indi[i];
				if (i != indi.size() - 1){
					cout << ",";
				}
			}
			cout << ")";


			if (tra > -1 && traObs > -1){

				TrajectoryObs<double, T> obs = m_trajectories[tra][traObs];
				int obs_size = (sizeof(obs.pos) / sizeof(*obs.pos));

				cout << " | CenterPoint: (";


				for (int i = 0; i < obs_size; i++){
					cout << obs.pos[i];
					if (i != obs_size - 1){
						cout << ",";
					}
				}
				cout << ")";
			}
			cout << endl;
		}
	}
};