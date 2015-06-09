<?php

namespace Studienprojekt\Base;

abstract class Algorithm {
  protected $trajectories = array();
  protected $dimension = 0;
  protected $results = array();

  protected $freespace = array();
  protected $freespace_size = 0;
  protected $shape_strides = array();

  public function __construct( $trajectories ) {
    $this->trajectories = $trajectories;
    $this->dimension = count( $this->trajectories );
  }

  public function run() {
    $real_dimension = $this->make_real_dimension();
    $add_value = $this->make_add_value();

    $this->shape_strides[0] = 1;
    $this->freespace_size = $this->trajectories[0]->get_length() + $add_value;

    for ( $i = 1; $i < $real_dimension; $i++ ) {
      $real_i = $this->make_real_i( $i );
      $this->shape_strides[ $i ] = $this->shape_strides[ $i - 1 ] * $this->trajectories[ ( $i - 1 ) % $this->dimension ]->get_length() + $add_value;
      $this->freespace_size *= $this->trajectories[ $real_i ]->get_length() + $add_value;
    }
  }

  protected function coords_to_index( $coords ) {
    $real_dimension = $this->make_real_dimension();
    if ( count( $coords ) !== $real_dimension ) {
      return -1;
    }
    
    $add_value = $this->make_add_value();

    $index = 0;
    for ( $i = $real_dimension - 1; $i >= 0; $i-- ) {
      $real_i = $this->make_real_i( $i );
      if ( $coords[ $i ] >= $this->trajectories[ $real_i ]->get_length() + $add_value ) {
        return -1;
      }
      $index += $coords[ $i ] * $this->shape_strides[ $i ];
    }

    return $index;
  }

  protected function index_to_coords( $index ) {
    $real_dimension = $this->make_real_dimension();
    $coords = array();

    for ( $i = 0; $i < $real_dimension; $i++ ) {
      $coords[] = -1;
    }

    if ( $index >= $this->freespace_size ) {
      return $coords;
    }

    for ( $i = $real_dimension - 1; $i >= 0; $i-- ) {
      $coords[ $i ] = intval( floor( $index / $this->shape_strides[ $i ] ) );
      $index = $index % $this->shape_strides[ $i ];
    }

    return $coords;
  }

  protected function add_coords( $coords1, $coords2 ) {
    if ( count( $coords1 ) != count( $coords2 ) ) {
      return $coords1;
    }
    $sum = array();
    for( $i = 0; $i < count( $coords1 ); $i++ ) {
      $sum[] = $coords1[ $i ] + $coords2[ $i ];
    }
    return $sum;
  }

  protected function is_in_boundaries( $coords, $min_coords, $max_coords ) {
    if ( count( $coords ) != count( $min_coords ) || count( $coords ) != count( $max_coords ) ) {
      return false;
    }
    for ( $i = 0; $i < count( $coords ); $i++ ) {
      if ( $coords[ $i ] < $min_coords[ $i ] || $coords[ $i ] > $max_coords[ $i ] ) {
        return false;
      }
    }
    return true;
  }

  protected function get_binary_choices( $start, $end, $length ) {
    $choices = array();
    for ( $i = $start; $i < $end; $i++ ) {
      $choices[] = $this->make_binary( $i, $length );
    }
    return $choices;
  }

  protected function make_binary( $value, $length ) {
    return array_map( 'intval', str_split( str_pad( decbin( $value ), $length, '0', STR_PAD_LEFT ), 1 ) );
  }

  protected function make_decimal( $binary ) {
    $result = 0;
    $counter = 0;
    for ( $i = count( $binary ) - 1; $i >= 0; $i-- ) {
      if ( $binary[ $i ] == 1 ) {
        $result += pow( 2, $counter );
      }
      $counter++;
    }

    return $result;
  }

  protected abstract function make_real_i( $i );

  protected abstract function make_real_dimension();

  protected abstract function make_add_value();

  public function get_results() {
    return $this->results;
  }
}
