#pragma once
#include <stdio.h>
#include <vector>
#include "trajectory.h"

using namespace std;

template <size_t T>
class base_algorithm{
	protected:

		//Attribute
		vector<Trajectory<double, T>> m_trajectories;
		int m_dimension = 0;
		
		int m_freespace_size = 0;
		vector<int> m_shape_strides;
		
		//Abstrakte Funktionen
		virtual int make_real_i(int i) = 0;
		virtual int make_real_dimension() = 0;

		//Funktionen
		int coords_to_index(vector<int> coords) {
			int real_dimension = make_real_dimension();
			if (coords.size() != real_dimension) {
				return -1;
			}

			int index = 0;
			for (int i = real_dimension - 1; i >= 0; i--) {
				int real_i = make_real_i(i);
				if (coords[i] >= m_trajectories[real_i].size()) {
					return -1;
				}
				index += coords[i] * m_shape_strides[i];
			}

			return index;
		}

		vector<int> index_to_coords(int index) {
			int real_dimension = make_real_dimension();
			vector<int> coords;
			for (int i = 0; i < real_dimension; i++) {
				coords.push_back(-1);
			}

			if (index >= m_freespace_size) {
				return coords;
			}

			for (int i = real_dimension - 1; i >= 0; i--) {
				coords[i] = (int)floor(index / m_shape_strides[i]);
				index = index % m_shape_strides[i];
			}
			return coords;
		}

		int modulo(int a, int b){
			if (a < 0){
				while (a < 0){
					a += b;
				}
			}
			return a % b;
		}

		vector<vector<int>> get_binary_choices(int start, int end, int len){
			vector<vector<int>> choices;

			for (int i = start; i < end; i++){
				vector<int> choice = make_Binary(i, len);
				choices.push_back(choice);
			}

			return choices;
		}

		vector<int> make_Binary(int value, int len){

			string binary = bitset<32>(value).to_string();
			binary = binary.substr(binary.length() - len, len);

			vector<int> choice;
			for (int i = 0; i < binary.length(); i++){
				choice.push_back(stoi(binary.substr(i, 1)));
			}

			return choice;
		}

		bool is_in_boundaries(vector<int> coords, vector<int> minCoords, vector<int> maxCoords){
			if (coords.size() != minCoords.size() || coords.size() != maxCoords.size()){
				return false;
			}
			for (int i = 0; i < coords.size(); i++){
				if (coords[i] < minCoords[i] || coords[i] > maxCoords[i]){
					return false;
				}
			}
			return true;
		}

		vector<int> add_coords(vector<int> coords1, vector<int> coords2) {
			vector<int> sum;

			if (coords1.size() != coords2.size()){
				return coords1;
			}

			for (int i = 0; i < coords1.size(); i++) {
				sum.push_back(coords1[i] + coords2[i]);
			}
			return sum;
		}

	public:
		base_algorithm(vector<Trajectory<double, T>> trajectories){
			m_trajectories = trajectories;
			m_dimension = trajectories.size();
		}

	  virtual void printResults() = 0;

	  void run(){
		  
		  int real_dimension = make_real_dimension();
		  m_shape_strides.push_back(1);
		  m_freespace_size = m_trajectories[0].size();

		  for (int i = 1; i < real_dimension; i++) {
			  int real_i = make_real_i(i);
			  
			  m_shape_strides.push_back(m_shape_strides[i - 1] * m_trajectories[modulo(real_i -1,m_dimension)].size());
			  m_freespace_size *= m_trajectories[real_i].size();
		  }
	  }
};