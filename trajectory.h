#pragma once

#ifndef __TRAJECTORY_H__
#define __TRAJECTORY_H__

#include<vector>
#include <string>

template<class T, size_t dim> struct TrajectoryObs {
	T pos[dim];
	T time;
    std::string trajectoryName;
    

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

	friend bool operator!= (const TrajectoryObs<T, dim>& obs1, const TrajectoryObs<T, dim>& obs2) {
		for (int i = 0; i < (int)dim; i++){
			if (obs1.pos[i] != obs2.pos[i]){
				return true;
			}
		}
		return false;
	}
};

template<class T, size_t dim> using Trajectory = std::vector<TrajectoryObs<T, dim>>;

// short names for 2- and 3-dimensional trajectories
template<class T> using Tr2 = Trajectory<T, 2>;
template<class T> using Tr3 = Trajectory<T, 3>;

#endif // __TRAJECTORY_H__
