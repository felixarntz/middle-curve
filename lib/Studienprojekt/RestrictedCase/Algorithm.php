<?php

namespace Studienprojekt\RestrictedCase;

class Algorithm extends \Studienprojekt\Base\Algorithm {
  protected $xboundaries = array();
  protected $xboundaries_size = 0;
  protected $xshape_strides = array();

  protected $current_epsilon = 0.0;

  public function run() {
    parent::run();

    $add_value = $this->make_add_value();

    $this->xshape_strides[0] = 1;
    $this->xboundaries_size = $this->trajectories[0]->get_length() + $add_value;

    for ( $i = 1; $i < $this->dimension - 1; $i++ ) {
      $this->xshape_strides[ $i ] = $this->xshape_strides[ $i - 1 ] * ( $this->trajectories[ ( $i - 1 ) % ( $this->dimension - 1 ) ]->get_length() + $add_value );
      $this->xboundaries_size *= $this->trajectories[ $i ]->get_length() + $add_value;
    }
    
    $this->fill_free_space();

    /*$path = $this->find_path();

    $this->results = array(
      'freespace'     => array(),
      'path'          => array(),
      'epsilon'       => $this->find_epsilon( $path ),
      'middle_curve'  => array_map( array( $this, 'get_point_for_output' ), $this->build_middle_curve( $path ) ),
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
    }*/
  }

  protected function fill_free_space() {
    for ( $i = 0; $i < $this->freespace_size; $i++ ) {
      $coords = $this->index_to_coords( $i );
      $this->freespace[ $i ] = new \Studienprojekt\RestrictedCase\XspacePoint( $coords );
      if ( $i == 0 ) {
        $this->freespace[ $i ]->enable( null );
      }
    }

    for ( $i = 0; $i < $this->xboundaries_size; $i++ ) {
      if ( $i == 0 ) {
        $this->xboundaries[ $i ] = 0;
      } else {
        $this->xboundaries[ $i ] = -1;
      }
    }

    $start_coords = array();
    for ( $x = 0; $x < $this->dimension; $i++ ) {
      $start_coords[ $x ] = 1;
    }

    for ( $i = $this->coords_to_index( $start_coords ); $i < $this->freespace_size; $i++ ) {
      $coords = $this->index_to_coords( $i );
      if ( $this->check_distance( $coords ) ) {
        if ( $this->freespace[ $i ]->get_value() ) {

          for ( $d = 0; $d < $this->dimension; $d++ ) {
            $upper_right_wedge = $this->get_upper_right_wedge( $d, $coords );
            $this->add_upper_right_wedge( $i, $coords, $d, $upper_right_wedge );
          }

        } else {

          for ( $d = 0; $d < $this->dimension; $d++ ) {
            $lower_left_wedge = $this->get_lower_left_wedge( $d, $coords );
            $extended_lower_left_wedge = $this->make_extended_lower_left_wedge( $lower_left_wedge );
            if ( $this->intersects( $extended_lower_left_wedge ) ) {
              $upper_right_wedge = $this->get_upper_right_wedge( $d, $coords );
              $this->add_lower_left_wedge( $i, $coords, $d, $lower_left_wedge );
              $this->add_upper_right_wedge( $i, $coords, $d, $upper_right_wedge );
            }
          }

        }
      }
    }
  }

  protected function add_upper_right_wedge( $i, $coords, $trajectory_index, $wedge ) {
    $max_last_coord = 0;
    foreach ( $wedge as $key => $wedge_coords ) {
      $wedge_xcoords = $this->coords_to_xcoords( $wedge_coords );
      if ( $wedge_coords[ $this->dimension - 1 ] > $this->xboundaries[ $this->xcoords_to_index( $wedge_xcoords ) ] ) {
        $this->freespace[ $this->coords_to_index( $wedge_coords ) ]->enable( $this->trajectories[ $trajectory_index ]->get_point( $coords[ $trajectory_index ] ) );
        $this->freespace[ $this->coords_to_index( $wedge_coords ) ]->set_previous( $i );
      }
      if ( $wedge_coords[ $this->dimension - 1 ] > $max_last_coord ) {
        $max_last_coord = $wedge_coords[ $this->dimension - 1 ];
      }
    }
    $xindex = $this->xcoords_to_index( $this->coords_to_xcoords( $coords ) );
    if ( $max_last_coord > $this->xboundaries[ $xindex ] ) {
      $this->xboundaries[ $xindex ] = $max_last_coord;
    }
  }

  protected function add_lower_left_wedge( $i, $coords, $trajectory_index, $wedge ) {
    $max_last_coord = 0;
    foreach ( $wedge as $key => $wedge_coords ) {
      $wedge_xcoords = $this->coords_to_xcoords( $wedge_coords );
      if ( $wedge_coords[ $this->dimension - 1 ] > $this->xboundaries[ $this->xcoords_to_index( $wedge_xcoords ) ] ) {
        $this->freespace[ $this->coords_to_index( $wedge_coords ) ]->enable( $this->trajectories[ $trajectory_index ]->get_point( $coords[ $trajectory_index ] ) );
        $this->freespace[ $i ]->set_previous( $this->coords_to_index( $wedge_coords ) );
      }
      if ( $wedge_coords[ $this->dimension - 1 ] > $max_last_coord ) {
        $max_last_coord = $wedge_coords[ $this->dimension - 1 ];
      }
    }
    $xindex = $this->xcoords_to_index( $this->coords_to_xcoords( $coords ) );
    if ( $max_last_coord > $this->xboundaries[ $xindex ] ) {
      $this->xboundaries[ $xindex ] = $max_last_coord;
    }
  }

  protected function get_upper_right_wedge( $trajectory_index, $coords ) {
    $wedge = array();

    $current_point = $this->trajectories[ $trajectory_index ]->get_point( $coords[ $trajectory_index ] );

    for ( $i = $this->coords_to_index( $coords ); $i < $this->freespace_size; $i++ ) {
      $wedge_coords = $this->index_to_coords( $wedge_coords );
      $add = true;
      for ( $d = 0; $d < $this->dimension; $d++ ) {
        if ( $wedge_coords[ $d ] < $coords[ $d ] ) {
          $add = false;
          break;
        }

        $point = $this->trajectories[ $d ]->get_point( $wedge_coords[ $d ] );
        if ( $this->calc_distance( $current_point->get_pos(), $point->get_pos() ) > $this->current_epsilon ) {
          $add = false;
          break;
        }
      }
      if ( $add ) {
        $wedge[] = $wedge_coords;
      }
    }

    return $wedge;
  }

  protected function get_lower_left_wedge( $trajectory_index, $coords ) {
    $wedge = array();

    $current_point = $this->trajectories[ $trajectory_index ]->get_point( $coords[ $trajectory_index ] );

    for ( $i = $this->coords_to_index( $coords ); $i >= 0; $i-- ) {
      $wedge_coords = $this->index_to_coords( $wedge_coords );
      $add = true;
      for ( $d = 0; $d < $this->dimension; $d++ ) {
        if ( $wedge_coords[ $d ] > $coords[ $d ] ) {
          $add = false;
          break;
        }

        $point = $this->trajectories[ $d ]->get_point( $wedge_coords[ $d ] );
        if ( $this->calc_distance( $current_point->get_pos(), $point->get_pos() ) > $this->current_epsilon ) {
          $add = false;
          break;
        }
      }
      if ( $add ) {
        $wedge[] = $wedge_coords;
      }
    }

    return $wedge;
  }

  protected function make_extended_lower_left_wedge( $wedge ) {
    $lowest_coords = $wedge[ count( $wedge ) - 1 ];
    foreach ( $wedge as $wedge_coords ) {
      for ( $i = 0; $i < count( $wedge_coords ); $i++ ) {
        if ( $wedge_coords[ $i ] < $lowest_coords[ $i ] ) {
          $lowest_coords = $wedge_coords;
          break;
        }
      }
    }

    $choices = $this->get_binary_choices( 1, pow( 2, $this->dimension ), $this->dimension );
    foreach ( $choices as $choice ) {
      $add_coords = array();
      for( $i = 0; $i < count( $choice ); $i++ ) {
        if ( $choice[ $i ] == 1 ) {
          $add_coords[] = -1;
        } else {
          $add_coords[] = 0;
        }
      }
      $wedge[] = $this->add_coords( $lowest_coords, $add_coords );
    }

    return $wedge;
  }

  protected function intersects( $wedge ) {
    return true;
  }

  protected function check_distance( $coords ) {
    return true;
  }

  protected function calc_distance( $pos1, $pos2 ) {
    return sqrt( pow( $pos1[0] - $pos2[0], 2 ) + pow( $pos1[1] - $pos2[1], 2 ) );
  }

  protected function coords_to_xcoords( $coords ) {
    $xcoords = array();
    for ( $i = 0; $i < $this->dimension - 1; $i++ ) {
      $xcoords[ $i ] = $coords[ $i ];
    }

    return $xcoords;
  }

  protected function xcoords_to_index( $coords ) {
    if ( count( $xcoords ) !== $this->dimension - 1 ) {
      return -1;
    }
    
    $add_value = $this->make_add_value();

    $index = 0;
    for ( $i = $this->dimension - 2; $i >= 0; $i-- ) {
      if ( $xcoords[ $i ] >= $this->trajectories[ $i ]->get_length() + $add_value ) {
        return -1;
      }
      $index += $xcoords[ $i ] * $this->xshape_strides[ $i ];
    }

    return $index;
  }

  protected function index_to_xcoords( $index ) {
    $xcoords = array();

    for ( $i = 0; $i < $this->dimension - 1; $i++ ) {
      $xcoords[] = -1;
    }

    if ( $index >= $this->xboundaries_size ) {
      return $xcoords;
    }

    for ( $i = $this->dimension - 2; $i >= 0; $i-- ) {
      $xcoords[ $i ] = intval( floor( $index / $this->xshape_strides[ $i ] ) );
      $index = $index % $this->xshape_strides[ $i ];
    }

    return $xcoords;
  }

  protected function make_real_i( $i ) {
    return $i;
  }

  protected function make_real_dimension() {
    return $this->dimension;
  }

  protected function make_add_value() {
    return 1;
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
