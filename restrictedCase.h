#pragma once
#include "base_algorithm.h"
#include "RS_Point.h"

#include <map>


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
	vector<double> xboundaries;
	int xboundaries_size = 0;
	vector<int> xshape_strides;

	double m_epsilon = 0.0;

	void fill_space(){
		
		xboundaries.erase(xboundaries.begin(), xboundaries.end());
		m_space.erase(m_space.begin(), m_space.end());
		
		for (int i = 0; i < m_freespace_size; i++){
			vector<int> coords = index_to_coords(i);
			m_space.push_back(RS_Point<T>(coords));
			
			if (i == 0){
				//Erster Value wird true gesetzt
				m_space[0].set_center(-1);
			}
		}
		
		for (int i = 0; i < xboundaries_size; i++){
			if (i == 0){
				xboundaries.push_back(0.0);
			}
			else{
				xboundaries.push_back(-1.0);
			}
		}

		vector<int> start_coords;
		for (int x = 0; x < m_dimension; x++){
			start_coords.push_back(1);
		}

		
		for (int i = coords_to_index(start_coords); i < m_freespace_size; i++){

			vector<int> coords = index_to_coords(i);

			if (check_distance(coords)){
				if (m_space[i].get_check()){

					for (int d = 0; d < m_dimension; d++){
						vector<vector<int>> upper_right_wedge = get_upper_right_wedge(d, coords);
						add_wedge(i, coords, d, upper_right_wedge, true); //true := upper_right_wedge,  false := lower_left_wedge
					}
				}
				else{

					for (int d = 0; d < m_dimension; d++){
						vector<vector<int>> lower_left_wedge = get_lower_left_wedge(d, coords);
						vector<vector<int>> extended_lower_left_wedge = make_extended_lower_left_wedge(lower_left_wedge);

						if (intersects(extended_lower_left_wedge)){
							vector<vector<int>> upper_right_wedge = get_upper_right_wedge(d, coords);
							add_wedge(i, coords, d, upper_right_wedge, true);
							add_wedge(i, coords, d, lower_left_wedge, false);
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

	vector<vector<int>> get_lower_left_wedge(int d, vector<int> coords){
		vector<vector<int>> wedge;

		if (coords[d] < 1){
			return wedge;
		}

		TrajectoryObs<double, T> current_point = m_trajectories[d][coords[d]];

		for (int i = coords_to_index(coords); i >= 0; i--){
			vector<int> wedge_coords = index_to_coords(i);
			bool add = true;

			for (int j = 0; j < m_dimension; j++){
				if (wedge_coords[j] > coords[j]){
					add = false;
					break;
				}

				if (wedge_coords[j] < 1){
					add = false;
					break;
				}

				TrajectoryObs<double, T> point = m_trajectories[j][wedge_coords[j]-1];
				if (calc_distance(current_point.pos, point.pos) > m_epsilon){
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

	vector<vector<int>> get_upper_right_wedge(int d, vector<int> coords){
		vector<vector<int>> wedge;

		if (coords[d] < 1){
			return wedge;
		}

		TrajectoryObs<double, T> current_point = m_trajectories[d][coords[d]-1];

		for (int i = coords_to_index(coords); i < m_freespace_size; i++){
			vector<int> wedge_coords = index_to_coords(i);
			bool add = true;

			for (int j = 0; j < m_dimension; j++){
				if (wedge_coords[j] < coords[j]){
					add = false;
					break;
				}

				if (wedge_coords[j] < 1){
					add = false;
					break;
				}

				TrajectoryObs<double, T> point = m_trajectories[j][wedge_coords[j]-1];
				if (calc_distance(current_point.pos, point.pos) > m_epsilon){
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

	vector<vector<int>> make_extended_lower_left_wedge(vector<vector<int>> wedge){
		
		vector<vector<int>> extended_temp;
		vector<int> lowest_coords = wedge[wedge.size() - 1]; //TO-DO

		for (auto tWedge : wedge){
			for (int i = 0; i < tWedge.size(); i++){
				if (tWedge[i] < lowest_coords[i]){
					lowest_coords = tWedge;
					break;
				}
			}
			extended_temp.push_back(tWedge);
		}

		vector<vector<int>> choices = get_binary_choices(1, pow(2, m_dimension), m_dimension);
		for (auto choice : choices){
			vector<int> add_c;

			for (int i = 0; i < m_dimension; i++){
				if (choice[i] == 1){
					add_c.push_back(-1);
				}
				else{
					add_c.push_back(0);
				}
			}

			extended_temp.push_back(add_coords(lowest_coords, add_c));
		}
		return extended_temp;
	}

	void add_wedge(int i, vector<int> coords, int d, vector<vector<int>> wedge, bool upper){
		int max_last_coord = 0;
		for (int i = 0; i < wedge.size(); i++){
			vector<int> wedge_xcoords = coords_to_xcoords(wedge[i]);
			if (wedge[i][m_dimension - 1] > xboundaries[xcoords_to_index(wedge_xcoords)]){
				m_space[coords_to_index(wedge[i])].set_center(d);

				if (upper){
					m_space[coords_to_index(wedge[i])].set_befor(&m_space[i]);
				}
				else{
					m_space[i].set_befor(&m_space[coords_to_index(wedge[i])]);
				}
			}

			if (wedge[i][m_dimension - 1] > max_last_coord){
				max_last_coord = wedge[i][m_dimension - 1];
			}
		}

		int xindex = xcoords_to_index(coords_to_xcoords(coords));
		if (max_last_coord > xboundaries[xindex]){
			xboundaries[xindex] = max_last_coord;
		}
	}

	bool intersects(vector<vector<int>> wedge){
		//TO-DO
		return true;
	}

	bool check_distance(vector<int> coords){
		
		for (int i = 0; i < coords.size(); i++){
			for (int j = 0; j < coords.size(); j++){

				if (coords[i] < 1 || coords[j] < 1){
					return false;
				}

				double temp_Distance = calc_distance(m_trajectories[i][coords[i]-1].pos, m_trajectories[j][coords[j]-1].pos);
				if (temp_Distance > m_epsilon){
					return false;
				}
			}
		}

		return true;
	}


	vector<int> coords_to_xcoords(vector<int> coords){
		vector<int> temp;
		for (int i = 0; i < coords.size() - 1; i++){
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

	vector<double> get_epsilon_list(){
		map<double, int> epList;

		for (int a = 0; a < m_trajectories.size(); a++){
			for (int b = 0; b < m_trajectories.size(); b++){
				for (int i = 0; i < m_trajectories[a].size(); i++){
					for (int j = 0; j < m_trajectories[b].size(); j++){
						if (a != b || i != j){
							double temp_Distance = calc_distance(m_trajectories[a][i].pos, m_trajectories[b][j].pos);
							epList.insert(pair<int, int>(temp_Distance, 0));
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

public:
	//Konstruktor
	RestrictedCase(vector<Trajectory<double, T>> trajectories) : base_algorithm<T>(trajectories){ }

	//Methoden
	void run(){
		base_algorithm<T>::run();

		cout << "1" << endl;

		int add_value = make_add_value();

		xshape_strides.push_back(1);
		xboundaries_size = m_trajectories[0].size() + add_value;
		
		cout << "2" << endl;

		for (int i = 1; i < m_dimension-1; i++) {
			xshape_strides.push_back(xshape_strides[i - 1] * (m_trajectories[modulo(i - 1, m_dimension-1)].size() + add_value));
			xboundaries_size *= m_trajectories[i].size() + add_value;
		}

		cout << "3" << endl;

		vector<double> epsilons = get_epsilon_list();
		for (int i = 0; i < epsilons.size(); i++){
			m_epsilon = epsilons[i];
			
			fill_space();

			vector<int> last_coords;

			for (int j = 0; j < m_dimension; j++){
				last_coords.push_back(m_trajectories[i].size());
			}

			if (m_space[coords_to_index(last_coords)].get_check()){
				break;
			}
		}
	}

};