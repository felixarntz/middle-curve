#pragma once

#include <math.h>
#include "discreteFrechet.h"

/*Unorderd Trajektory Class*/

//=============================================================================

//Konstruktore



//=============================================================================

//get- set- Funktionen
/*adds a new Trajectory to vector of mutiple Trajectory*/
/*template<T>
void DiscreteFrechet<T>::addTrajectory(Trajectory<double, T> field){
	m_traject.push_back(field);
}*/


// creates a union of points containing all points around Epsilon
/*template<T>
vector<TrajectoryObs<double, T>> DiscreteFrechet<T>::getUnion(TrajectoryObs<double, T> point, int depth){
	if (depth >= 0){
		vector<TrajectoryObs<double, 2>> uni;
		for (auto it : m_traject){
			for (int i = 0; i < it.size(); i++){
				if (calcDistance(point, it[i]) <= m_epsilon){
					uni.push_back(it[i]);
				}
			}
		}

		if (depth == 0){
			return uni;
		}
		else{
			vector<TrajectoryObs<double, 2>> itTemp = uni;
			for (auto it : itTemp){
				vector<TrajectoryObs<double, 2>> temp = getUnion(it, (depth - 1));
				for (auto itS : temp){
					uni.push_back(itS);
				}
			}

			return uni;
		}
	}
	return vector<TrajectoryObs<double, 2>>();
}*/


/*
template<size_t T>
vector<TrajectoryObs<double, T>> DiscreteFrechet<T>::findPath(int x, int y){
	
	vector<TrajectoryObs<int, 2>> result;
	vector<TrajectoryObs<int, 2>> result2;
	vector<TrajectoryObs<int, 2>> result3;
	vector<TrajectoryObs<int, 2>> partPath;

	if (x >= 0 && y >= 0 && x < m_freeSpaceBoolean.size() && y < m_freeSpaceBoolean[0].size()){
		
		if (m_freeSpaceBoolean[x][y] == true){

			TrajectoryObs<int, 2> now;
			now.pos[0] = x;
			now.pos[1] = y;

			partPath.push_back(now);

			if (x + 1 < m_freeSpaceBoolean.size() || y + 1 < m_freeSpaceBoolean[0].size()){
				if (m_epsilon > 0){
					result = findPath(x + 1, y);
					if (result[0].pos[0] == -1){
						result = findPath(x, y + 1);
						if (result[0].pos[0] == -1){
							result = findPath(x + 1, y + 1);
							if (result[0].pos[0] == -1){
								return result;
							}
						}
					}
				}
				else{

					double val = 100000.0;
					result = findPath(x + 1, y);
					if (result[0].pos[0] != -1){
						if (m_freeSpaceDijkstraDistance[x + 1][y] < val){
							val = m_freeSpaceDijkstraDistance[x + 1][y];
						}
					}

					result2 = findPath(x, y + 1);
					if (result2[0].pos[0] != -1){
						if (m_freeSpaceDijkstraDistance[x][y + 1] < val){
							val = m_freeSpaceDijkstraDistance[x][y + 1];
							result = result2;
						}
					}

					result3 = findPath(x + 1, y + 1);
					if (result3[0].pos[0] != -1){
						if (m_freeSpaceDijkstraDistance[x + 1][y + 1] < val){
							val = m_freeSpaceDijkstraDistance[x + 1][y + 1];
							result = result3;
						}
					}

					if (m_freeSpaceDijkstraBoolean[x][y] == false){
						m_freeSpaceDijkstraDistance[x][y] += val;
						m_freeSpaceDijkstraBoolean[x][y] = true;
					}
				}
			}
			else{
				return partPath;
			}
			
			for (auto it : result){
				partPath.push_back(it);
			}
		}
		else{
			result.push_back(m_def);
			return result;
		}
		return partPath;
	}
	result.push_back(m_def);
	return result;
	
}*/
