<?php

namespace Studienprojekt\OrderedCase;

class Algorithm extends \Studienprojekt\Base\Algorithm {
  protected $infinite = 1000000000000.0;

  public function run() {
    parent::run();

    $this->fill_free_space();

    $path = $this->find_path();

    $this->results = array(
      'boolspace'     => array(),
      'path'          => array(),
      'epsilon'       => $this->find_epsilon( $path ),
    );

    foreach ( $this->freespace as $key => $boolspace_point ) {
      $this->results['boolspace'][ $key ] = array(
        'coords'          => $boolspace_point->get_indices(),
        'points'          => $this->get_points_for_output( $boolspace_point->get_indices() ),
        'cover_points'    => $this->get_points_for_output( $boolspace_point->get_indices(), 'cover' ),
        'values'          => $boolspace_point->get_values(),
      );
    }
    foreach ( $path as $key => $boolspace_point ) {
      $this->results['path'][ $key ] = array(
        'coords'          => $boolspace_point->get_indices(),
        'mainvalue'       => $boolspace_point->get_mainvalue(),
      );
    }
  }

  protected function fill_free_space() {
    for ( $i = 0; $i < $this->freespace_size; $i++ ) {
      $coords = $this->index_to_coords( $i );
      $this->freespace[ $i ] = new \Studienprojekt\OrderedCase\BoolspacePoint( $coords, $this->infinite );
      $values = array();

      // Fall A (1 value) --> Regel 1
      $values[] = $this->rule_1( $coords );

      // Fall B/D (2^k - 2 values) --> Regel 2
      for ( $j = 1; $j <= intval( pow( 2, $this->dimension ) - 2 ); $j++ ) {
        $values[] = $this->rule_2( $coords, $j );
      }

      // Fall C/E (k values) --> Regel 3
      for ( $j = 1; $j <= $this->dimension; $j++ ) {
        $values[] = $this->rule_3( $coords, $j );
      }

      // Fall F/G (k * (2^(k-1) - 1) values) --> Regel 4
      for ( $j = 1; $j <= $this->dimension; $j++ ) {
        for ( $k = 1; $k <= intval( pow( 2, $this->dimension - 1 ) - 1 ); $k++ ) {
          $values[] = $this->rule_4( $coords, $j, $k );
        }
      }

      $this->freespace[ $i ]->set_values( $values );
    }
  }

  protected function rule_1( $coords ) {
    // gib true zurück, falls alle Koordinaten 0 sind
    if ( $this->coords_to_index( $coords ) == 0 ) {
      return 0.0;
    }

    $all_zero = true;
    $choice = array();
    for( $i = 0; $i < $this->dimension; $i++ ) {
      if ( $coords[ $i ] > 0 ) {
        $choice[] = 1;
        $all_zero = false;
      } else {
        $choice[] = 0;
      }
    }

    // gib false zurück, falls alle vorderen Koordinaten 0 sind
    if ( $all_zero ) {
      return $this->infinite;
    }

    // Rück-Referenz auf Gesamtregel für alle vorderen Koordinaten > 0 um 1 verringert
    return $this->freespace[ $this->coords_to_index( $this->add_coords( $coords, $this->get_add_coords( $choice, 1 ) ) ) ]->get_mainvalue();
  }

  protected function rule_2( $coords, $rule_counter = 0 ) {
    // Fehlerrückgabe: rule_counter beginnt immer bei 1
    if ( $rule_counter == 0 ) {
      return $this->infinite;
    }

    // gib false zurück, falls aktuelle Koordinate 0 oder alle anderen vorderen Koordinaten 0
    $choice = $this->make_binary( $rule_counter, $this->dimension );
    $others = false;
    foreach ( $choice as $key => $value ) {
      if ( $value == 1 && $coords[ $key ] == 0 ) {
        return $this->infinite;
      } elseif ( $value == 0 && $coords[ $key ] > 0 ) {
        $others = true;
      }
    }
    if ( ! $others ) {
      return $this->infinite;
    }

    $add_coords = $this->get_add_coords( $choice, 2 );

    $added_index = $this->coords_to_index( $this->add_coords( $coords, $add_coords ) );

    $values_to_compare = array();

    // Rück-Referenz auf Regel 2 an derselben Stelle
    $values_to_compare[] = $this->freespace[ $added_index ]->get_value_at( $this->get_rule_index( $rule_counter, 2 ) );

    // Rück-Referenzen auf Regel 3/4 für alle Stellen 0 der Binärzahl
    foreach ( $choice as $key => $value ) {
      if ( $value == 0 ) {
        $subcounter = $this->make_decimal( $this->strip_choice_index( $key, $choice ) );
        if ( $subcounter == 0 ) { //TODO: Zugriff auf Regel 3? Oder gar kein Zugriff?
          $values_to_compare[] = $this->freespace[ $added_index ]->get_value_at( $this->get_rule_index( $key + 1, 3 ) );
        } else {
          $values_to_compare[] = $this->freespace[ $added_index ]->get_value_at( $this->get_rule_index( $key + 1, 4, $subcounter ) );
        }
      }
    }
    
    // gib Minimalwert aus Rück-Referenzen zurück
    return min( $values_to_compare );
  }

  protected function rule_3( $coords, $rule_counter = 0 ) {
    // Fehlerrückgabe: rule_counter beginnt immer bei 1
    if ( $rule_counter == 0 ) {
      return $this->infinite;
    }

    // gib false zurück, falls aktuelle Koordinate 0
    if ( $coords[ $rule_counter - 1 ] == 0 ) {
      return $this->infinite;
    }

    // gib false zurück, falls eine der hinteren Koordinaten 0
    for ( $i = $this->dimension; $i < $this->dimension * 2; $i++ ) {
      if ( $coords[ $i ] == 0 ) {
        return $this->infinite;
      }
    }

    $values_to_compare = array();

    // gib false zurück, falls einer der Distanzvergleiche mit Epsilon false ergibt
    $current_point = $this->trajectories[ $rule_counter - 1 ]->get_point( $coords[ $rule_counter - 1 ] - 1 );
    for ( $x = 0; $x < $this->dimension; $x++ ) {
      $point = $this->trajectories[ $x ]->get_point( $coords[ $x + $this->dimension ] - 1 );
      $values_to_compare[] = $this->calc_distance( $point->get_pos(), $current_point->get_pos() );
    }

    $subvalues_to_compare = array();

    // Rück-Referenz auf Regel 1
    $add_coords = $this->get_add_coords( $this->make_binary( 0, $this->dimension ), 3 );
    $subvalues_to_compare[] = $this->freespace[ $this->coords_to_index( $this->add_coords( $coords, $add_coords ) ) ]->get_value_at( $this->get_rule_index( 1, 1 ) );

    // Rück-Referenzen auf Regel 3 an derselben Stelle für alle Binärkombinationen > 0
    $choices = $this->get_binary_choices( 0, intval( pow( 2, $this->dimension ) - 1 ), $this->dimension );
    foreach ( $choices as $choice ) {
      $add_coords = $this->get_add_coords( $choice, 3 );
      $subvalues_to_compare[] = $this->freespace[ $this->coords_to_index( $this->add_coords( $coords, $add_coords ) ) ]->get_value_at( $this->get_rule_index( $rule_counter, 3 ) );
    }

    $values_to_compare[] = min( $subvalues_to_compare );

    // gib Maximalwert aus Rück-Referenzen zurück
    return max( $values_to_compare );
  }

  protected function rule_4( $coords, $rule_counter = 0, $inner_rule_counter = 0 ) {
    // Fehlerrückgabe: rule_counter beginnt immer bei 1
    if ( $rule_counter == 0 ) {
      return $this->infinite;
    }

    // gib false zurück, falls aktuelle Koordinate 0
    if ( $coords[ $rule_counter - 1 ] == 0 ) {
      return $this->infinite;
    }

    // gib false zurück, falls eine der hinteren Koordinaten 0
    for ( $i = $this->dimension; $i < $this->dimension * 2; $i++ ) {
      if ( $coords[ $i ] == 0 ) {
        return $this->infinite;
      }
    }

    // gib false zurück, falls eine der vorderen Koordinaten 0 und die entsprechende hintere Koordinate 1
    for( $i = 0; $i < $this->dimension; $i++ ) {
      if ( $coords[ $i ] == 0 && $coords[ $i + $this->dimension ] == 1 ) {
        return $this->infinite;
      }
    }

    $values_to_compare = array();

    // gib false zurück, falls einer der Distanzvergleiche mit Epsilon false ergibt
    $current_point = $this->trajectories[ $rule_counter - 1 ]->get_point( $coords[ $rule_counter - 1 ] - 1 );
    for ( $x = 0; $x < $this->dimension; $x++ ) {
      $point = $this->trajectories[ $x ]->get_point( $coords[ $x + $this->dimension ] - 1 );
      $values_to_compare[] = $this->calc_distance( $point->get_pos(), $current_point->get_pos() );
    }

    $subvalues_to_compare = array();

    $add_coords = $this->get_add_coords( $this->make_binary( 0, $this->dimension ), 4 );

    $added_index = $this->coords_to_index( $this->add_coords( $coords, $add_coords ) );

    $choice = $this->make_binary( $inner_rule_counter, $this->dimension - 1 );

    // Rück-Referenz auf Regel 2
    $rule_2_index = $this->make_decimal( $this->insert_choice_index( $rule_counter - 1, $choice ) );
    $subvalues_to_compare[] = $this->freespace[ $added_index ]->get_value_at( $this->get_rule_index( $rule_2_index, 2 ) );

    // Rück-Referenzen auf Regel 3/4 für alle Stellen 1 der Binärzahl
    foreach ( $choice as $key => $value ) {
      if ( $value == 1 ) {
        $subcounter = 0;
        if ( $rule_counter - 1 > $key ) {
          $subcounter = $this->make_decimal( $this->strip_choice_index( $key, $this->insert_choice_index( $rule_counter - 1, $choice ) ) );
        } else {
          $subcounter = $this->make_decimal( $this->insert_choice_index( $rule_counter - 1, $this->strip_choice_index( $key, $choice ) ) );
        }
        if ( $subcounter == 0 ) {
          $subvalues_to_compare[] = $this->freespace[ $added_index ]->get_value_at( $this->get_rule_index( $key + 1, 3 ) );
        } else {
          $subvalues_to_compare[] = $this->freespace[ $added_index ]->get_value_at( $this->get_rule_index( $key + 1, 4, $subcounter ) );
        }
      }
    }

    // Rück-Referenzen auf Regel 4 an derselben Stelle für jeweils eine der hinteren Stellen -1 gesetzt
    for ( $i = 0; $i < $this->dimension; $i++ ) {
      $add_coords = $this->get_add_coords( $this->make_binary( intval( pow( 2, $i ) ), $this->dimension ), 4 );
      $subvalues_to_compare[] = $this->freespace[ $this->coords_to_index( $this->add_coords( $coords, $add_coords ) ) ]->get_value_at( $this->get_rule_index( $rule_counter, 4, $inner_rule_counter ) );
    }

    $values_to_compare[] = min( $subvalues_to_compare );

    // gib Maximalwert aus Rück-Referenzen zurück
    return max( $values_to_compare );
  }

  protected function get_rule_index( $rule_counter, $rule = 0, $inner_rule_counter = 0 ) {
    if ( $rule < 1 || $rule > 4 ) {
      return -1;
    }

    if ( $rule == 1 ) {
      return 0;
    }

    if ( $rule_counter < 1 ) {
      return -1;
    }

    $rule_counter -= 1;

    $index = 1;

    if ( $rule > 2 ) {
      $index += pow( 2, $this->dimension ) - 2;
    }
    if ( $rule > 3 ) {
      $index += $this->dimension + ( $rule_counter - 1 ) * ( pow( 2, $this->dimension - 1 ) - 1 );
      return $index + $inner_rule_counter;
    }

    return $index + $rule_counter;
  }

  protected function find_epsilon( $path ) {
    $epsilon = 0.0;
    foreach ( $path as $freespace_point ) {
      $distance = $freespace_point->get_mainvalue();
      if ( $distance > $epsilon ) {
        $epsilon = $distance;
      }
    }
    return $epsilon;
  }

  protected function find_path() {
    $start_coords = array();
    for ( $i = 0; $i < $this->dimension; $i++ ) {
      $start_coords[] = 1;
    }
    for ( $i = 0; $i < $this->dimension; $i++ ) {
      $start_coords[] = $this->trajectories[ $i ]->get_length();
    }
    $start_index = $this->coords_to_index( $start_coords );
    $this->find_cheapest_path( $start_index, $this->get_binary_choices( 1, intval( pow( 2, $this->dimension ) ), $this->dimension ) );
    $index = $start_index;
    $path = array();
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
            $this->find_cheapest_path( $next_index, $choices );
            if ( $this->freespace[ $next_index ]->get_cost() < $cheapest ) {
              $rest_index = $next_index;
              $cheapest = $this->freespace[ $next_index ]->get_cost();
            }
          }
        }
        $this->freespace[ $index ]->set_next( $rest_index, $cheapest );
      }
    }
  }

  protected function insert_choice_index( $index, $choice ) {
    $new_choice = array();
    if ( $index == count( $choice ) ) {
      $new_choice = $choice;
      $new_choice[] = 0;
    } else {
      foreach ( $choice as $key => $value ) {
        if ( $key == $index ) {
          $new_choice[] = 0;
        }
        $new_choice[] = $value;
      }
    }
    return $new_choice;
  }

  protected function strip_choice_index( $index, $choice ) {
    $new_choice = array();
    foreach ( $choice as $key => $value ) {
      if ( $key != $index ) {
        $new_choice[] = $value;
      }
    }
    return $new_choice;
  }

  protected function get_add_coords( $binary_choice, $rule = 0 ) {
    $add_coords = array();
    switch ( $rule ) {
      case 4:
      case 3:
        for ( $i = 0; $i < $this->dimension; $i++ ) {
          $add_coords[] = 0;
        }
        for ( $i = 0; $i < $this->dimension; $i++ ) {
          if ( $binary_choice[ $i ] == 0 ) {
            $add_coords[] = -1;
          } else {
            $add_coords[] = 0;
          }
        }
        break;
      case 2:
      case 1:
        for ( $i = 0; $i < $this->dimension; $i++ ) {
          if ( $binary_choice[ $i ] == 1 ) {
            $add_coords[] = -1;
          } else {
            $add_coords[] = 0;
          }
        }
        for ( $i = 0; $i < $this->dimension; $i++ ) {
          $add_coords[] = 0;
        }
        break;
      default:
        for ( $i = 0; $i < $this->dimension * 2; $i++ ) {
          $add_coords[] = 0;
        }
    }
    return $add_coords;
  }

  protected function calc_distance( $pos1, $pos2 ) {
    return sqrt( pow( $pos1[0] - $pos2[0], 2 ) + pow( $pos1[1] - $pos2[1], 2 ) );
  }

  protected function get_points_for_output( $coords, $mode = 'default' ) {
    $points = array();
    $start = 0;
    $end = $this->dimension;
    $subtract = 0;
    if ( $mode == 'cover' ) {
      $start += $this->dimension;
      $end += $this->dimension;
      $subtract += $this->dimension;
    }
    for ( $i = $start; $i < $end; $i++ ) {
      if ( $coords[ $i ] > 0 ) {
        $points[] = $this->get_point_for_output( $this->trajectories[ $i - $subtract ]->get_point( $coords[ $i ] - 1 ) );
      }
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

  protected function make_real_i( $i ) {
    if ( $i >= $this->dimension ) {
      return $i - $this->dimension;
    }
    return $i;
  }

  protected function make_real_dimension() {
    return $this->dimension * 2;
  }

  protected function make_add_value() {
    return 1;
  }
}
