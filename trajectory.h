#pragma once

#ifndef __TRAJECTORY_H__
#define __TRAJECTORY_H__

#include<vector>

template<class T, size_t dim> struct TrajectoryObs {
	T pos[dim];
	T time;

	// prints a string of the form (x_1, x_2, ..., x_d) t=<time>
	friend std::ostream& operator<< (std::ostream& os, const TrajectoryObs<T, dim>& obs) {
		os << "(";
		for (size_t i = 0; i < dim; i++) {
			os << (i ? ", " : "") << obs.pos[i];
		}
		os << ")";
		os << " t=" << obs.time;
		return os;
	}
};

template<class T, size_t dim> using Trajectory = std::vector<TrajectoryObs<T, dim>>;

// short names for 2- and 3-dimensional trajectories
template<class T> using Tr2 = Trajectory<T, 2>;
template<class T> using Tr3 = Trajectory<T, 3>;

#endif // __TRAJECTORY_H__
