<?php

namespace Studienprojekt\DiscreteFrechet;

class Algorithm extends \Studienprojekt\Base\Algorithm {

  public function run() {
    parent::run();
    
    $this->fill_free_space();

    $path = $this->find_path();

    $this->results = array(
      'freespace'   => array(),
      'path'        => array(),
      'epsilon'     => $this->find_epsilon( $path ),
    );
    foreach ( $this->freespace as $key => $freespace_point ) {
      $this->results['freespace'][ $key ] = array(
        'coords'          => $freespace_point->get_indices(),
        'points'          => $this->get_points_for_output( $freespace_point->get_indices() ),
        'center_point'    => $this->get_point_for_output( $freespace_point->get_center_point() ),
        'center_distance' => $freespace_point->get_center_distance(),
      );
    }
    foreach ( $path as $key => $freespace_point ) {
      $this->results['path'][ $key ] = array(
        'coords'          => $freespace_point->get_indices(),
        'center_point'    => $this->get_point_for_output( $freespace_point->get_center_point() ),
        'center_distance' => $freespace_point->get_center_distance(),
      );
    }
  }

  protected function fill_free_space() {
    for ( $i = 0; $i < $this->freespace_size; $i++ ) {
      $coords = $this->index_to_coords( $i );
      $this->freespace[ $i ] = new \Studienprojekt\DiscreteFrechet\FreespacePoint( $coords );
      $max_distance = 0.0;
      for ( $j = 0; $j < $this->dimension; $j++ ) {
        $current_point = $this->trajectories[ $j ]->get_point( $coords[ $j ] );
        for ( $k = 0; $k < $this->dimension; $k++ ) {
          if ( $k != $j ) {
            $point = $this->trajectories[ $k ]->get_point( $coords[ $k ] );
            $distance = $this->calc_distance( $point->get_pos(), $current_point->get_pos() );
            if ( $distance > $max_distance ) {
              $max_distance = $distance;
            }
          }
        }
      }
      $this->freespace[ $i ]->set_center_point( $this->trajectories[0]->get_point( $coords[0] ), $max_distance );
    }
  }

  protected function find_epsilon( $path ) {
    $epsilon = 0.0;
    foreach ( $path as $freespace_point ) {
      $distance = $freespace_point->get_center_distance();
      if ( $distance > $epsilon ) {
        $epsilon = $distance;
      }
    }
    return $epsilon;
  }

  protected function find_path() {
    $path = array();
    $this->find_cheapest_path( 0, $this->get_binary_choices( 1, intval( pow( 2, $this->dimension ) ), $this->dimension ) );
    $index = 0;
    while ( $index > -1 ) {
      $current = $this->freespace[ $index ];
      $path[] = $current;
      $index = $current->get_next();
    }
    return $path;
  }

  protected function find_cheapest_path( $index, $choices ) {
    if ( $index == $this->freespace_size - 1 ) {
      $this->freespace[ $index ]->set_next( -1, 0.0 );
    } else {
      if ( ! $this->freespace[ $index ]->get_visited() ) {
        $rest_index = -1;
        $cheapest = 100000000000.0;
        foreach ( $choices as $choice ) {
          $next_coords = $this->add_coords( $this->index_to_coords( $index ), $choice );
          $next_index = $this->coords_to_index( $next_coords );
          if ( $next_index > -1 ) {
            $current_index = $this->find_cheapest_path( $next_index, $choices );
            if ( $this->freespace[ $current_index ]->get_cost() < $cheapest ) {
              $rest_index = $current_index;
              $cheapest = $this->freespace[ $current_index ]->get_cost();
            }
          }
        }
        $this->freespace[ $index ]->set_next( $rest_index, $cheapest );
      }
    }
  }

  /*protected function find_cheapest_path( $index, $choices ) {
    $path = array();
    $rest_path = array();

    if ( $index == $this->freespace_size - 1 ) {
      $this->freespace[ $index ]->set_rest_path( array(), 0.0 );
      $this->freespace[ $index ]->set_visited();
    } else {
      if ( $this->freespace[ $index ]->get_visited() ) {
        $rest_path = $this->freespace[ $index ]->get_rest_path();
      } else {
        $rest_path = array();
        $cheapest = 1000000000.0;
        foreach ( $choices as $choice ) {
          $next_coords = $this->add_coords( $this->index_to_coords( $index ), $choice );
          $next_index = $this->coords_to_index( $next_coords );
          if ( $next_index > -1 ) {
            $current_path = $this->find_cheapest_path( $next_index, $choices );
            if ( $current_path[0]->get_cost() < $cheapest ) {
              $rest_path = $current_path;
              $cheapest = $current_path[0]->get_cost();
            }
          }
        }
        $this->freespace[ $index ]->set_rest_path( $rest_path, $cheapest );
        $this->freespace[ $index ]->set_visited();
      }
    }
    $path[] = $this->freespace[ $index ];
    return array_merge( $path, $rest_path );
  }*/

  protected function calc_distance( $pos1, $pos2 ) {
    return sqrt( pow( $pos1[0] - $pos2[0], 2 ) + pow( $pos1[1] - $pos2[1], 2 ) );
  }

  protected function make_real_i( $i ) {
    return $i;
  }

  protected function make_real_dimension() {
    return $this->dimension;
  }

  protected function make_add_value() {
    return 0;
  }

  protected function get_points_for_output( $coords ) {
    $points = array();
    for ( $i = 0; $i < $this->dimension; $i++ ) {
      $points[ $i ] = $this->get_point_for_output( $this->trajectories[ $i ]->get_point( $coords[ $i ] ) );
    }
    return $points;
  }

  protected function get_point_for_output( $point ) {
    return array(
      'index'           => $point->get_index(),
      'trajectory_name' => $this->trajectories[ $point->get_trajectory_index() ]->get_name(),
      'pos'             => array(
        'x'               => $point->get_pos_x(),
        'y'               => $point->get_pos_y(),
      ),
      'time'            => $point->get_time(),
    );
  }
}
