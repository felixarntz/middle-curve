#pragma once
#include <stdio.h>
#include <vector>
#include "trajectory.h"

using namespace std;

template <size_t T>
class base_algorithm{
	protected:

		vector<Trajectory<double, T>> m_trajectories;
		int m_dimension = 0;
		
		int m_freespace_size = 0;
		vector<int> m_shape_strides;

		virtual int make_real_i(int i) = 0;
		virtual int make_real_dimension() = 0;


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