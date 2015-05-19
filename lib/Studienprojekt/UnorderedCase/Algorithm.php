<?php

namespace Studienprojekt\UnorderedCase;

class Algorithm extends \Studienprojekt\DiscreteFrechet\Algorithm {
  protected function fill_free_space() {
    for ( $i = 0; $i < $this->freespace_size; $i++ ) {
      $coords = $this->index_to_coords( $i );
      $this->freespace[ $i ] = new \Studienprojekt\UnorderedCase\FreespacePoint( $coords );
      foreach ( $this->trajectories as $trajectory ) {
        $points = $trajectory->get_points();
        foreach ( $points as $point ) {
          $max_distance = 0.0;
          for ( $j = 0; $j < $this->dimension; $j++ ) {
            $current_point = $this->trajectories[ $j ]->get_point( $coords[ $j ] );
            $distance = $this->calc_distance( $point->get_pos(), $current_point->get_pos() );
            if ( $distance > $max_distance ) {
              $max_distance = $distance;
            }
          }
          $this->freespace[ $i ]->set_center_point( $point, $max_distance );
        }
      }
    }
  }
}
