<?php

namespace Studienprojekt\OrderedCase;

class Algorithm extends \Studienprojekt\Base\Algorithm {
  protected $boolsize = 0;

  public function run() {
    parent::run();
    
    $this->boolsize = pow( 2, $this->dimension - 1 ) * ( $this->dimension + 2 ) - 1;

    $this->results = array();
  }

  protected function fill_free_space() {
    for ( $i = 0; $i < $this->freespace_size; $i++ ) {
      $coords = $this->index_to_coords( $i );
      $this->freespace[ $i ] = new \Studienprojekt\OrderedCase\BoolspacePoint( $coords );
      $boolvalues = array();
      $counter = 0;

      // Fall A (1 boolvalue)
      if ( $i == 0 ) {
        $boolvalues[] = true;
      } else {
        $choice = $this->make_binary( 0, $this->dimension );
        $calc_coords = $this->get_calc_coords( $choice );
        if ( $this->is_in_boundaries( $coords, $calc_coords[0], $calc_coords[1] ) ) {
          $boolvalues[] = false;
        } else {
          $choices = $this->get_binary_choices( 1, intval( pow( 2, $this->dimension ) ), $this->dimension );
          foreach ( $choices as $choice ) {
            $calc_coords = $this->get_calc_coords( $choice );
            if ( $this->is_in_boundaries( $coords, $calc_coords[0], $calc_coords[1] ) ) {
              $boolvalues[] = $this->freespace[ $this->coords_to_index( $this->add_coords( $coords, $calc_coords[2] ) ) ]->get_mainvalue();
              break;
            }
          }
        }
      }

      $counter++;

      // Fall B/D (2^k - 2 boolvalues)
      $choices = $this->get_binary_choices( 1, intval( pow( 2, $this->dimension ) - 1 ), $this->dimension );
      for ( $choices as $choice ) {
        //$boolvalues[] = false;
        $counter++;
      }

      for ( $j = 0; $j < $this->dimension; $j++ ) {
        // Fall C/E (k boolvalues)
        $choice = $this->make_binary( 0, $this->dimension - 1 );
        //$boolvalues[] = false;
        $counter++;

        // Fall F/G (k * (2^(k-1) - 1) boolvalues)
        $choices = $this->get_binary_choices( 1, intval( pow( 2, $this->dimension - 1 ) ), $this->dimension - 1 );
        foreach ( $choices as $choice ) {
          //$boolvalues[] = false;
          $counter++;
        }
      }

      //$this->freespace[ $i ]->set_boolvalues( $boolvalues );
    }
  }

  protected function get_calc_coords( $binary_choice ) {
    $min_coords = array();
    $max_coords = array();
    $add_coords = array();
    for ( $i = 0; $i < $this->dimension; $i++ ) {
      if ( $binary_choice[ $i ] == 1 ) {
        $max_coords[] = count( $this->trajectories[ $i ] ) - 1;
        $add_coords[] = -1;
      } else {
        $max_coords[] = 0;
        $add_coords[] = 0;
      }
      $min_coords[] = 0;
    }
    for ( $i = 0; $i < $this->dimension; $i++ ) {
      $max_coords[] = count( $this->trajectories[ $i ] ) - 1;
      $add_coords[] = 0;
      $min_coords[] = 0;
    }
    return array( $min_coords, $max_coords, $add_coords );
  }

  protected function make_real_i( $i ) {
    if ( $i >= $this->dimension ) {
      return $i - $this->dimension;
    }
    return $i;
  }

  protected function make_real_dimension() {
    return $this->dimension * 2;
  }
}
