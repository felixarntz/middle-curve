#pragma once

#include "FS_Point.h"
#include "discreteFrechet.h"

template<size_t T>
class UnorderedCase : public DiscreteFrechet<T> {

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

	using DiscreteFrechet<T>::m_freespace;

public:
	UnorderedCase(vector<Trajectory<double, T>> field) : DiscreteFrechet<T>(field){ }

	/**/
	void fill_free_space() {

		for (int i = 0; i < m_freespace_size; i++) {
			vector<int> coords = index_to_coords(i);
			m_freespace.push_back(FS_Point<T>(coords));

			for (int j = 0; j < m_dimension; j++) {

				Trajectory<double, T> points = m_trajectories[j];
				//TrajectoryObs<double, T> current_point = m_trajectories[j][coords[j]];

				for (int k = 0; k < points.size(); k++){
					double max_distance = 0.0;
					for (int a = 0; a < m_dimension; a++){
						TrajectoryObs<double, T> current_point = m_trajectories[a][coords[a]];
						double temp_Distance = calc_distance(points[k].pos, current_point.pos);
						if (temp_Distance > max_distance) {
							max_distance = temp_Distance;
						}
					}
					m_freespace[i].set_center_point(points[k], max_distance);
				}
			}
		}
	}
};