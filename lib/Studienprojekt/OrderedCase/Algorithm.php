<?php

namespace Studienprojekt\OrderedCase;

class Algorithm extends \Studienprojekt\Base\Algorithm {
  public function run() {
    parent::run();

    $this->results = array();
  }

  protected function rule_1( $coords, $rule_counter = 0 ) {
    if ( $this->coords_to_index( $coords ) == 0 ) {
      return true;
    }

    $choice = $this->make_binary( 0, $this->dimension );
    $boundary_coords = $this->get_boundary_coords( $choice, 1 );
    if ( $this->is_in_boundaries( $coords, $boundary_coords[0], $boundary_coords[1] ) ) {
      return false;
    }

    $choices = $this->get_binary_choices( 1, intval( pow( 2, $this->dimension ) ), $this->dimension );
    foreach ( $choices as $choice ) {
      $boundary_coords = $this->get_boundary_coords( $choice, 1 );
      if ( $this->is_in_boundaries( $coords, $boundary_coords[0], $boundary_coords[1] ) ) {
        return $this->freespace[ $this->coords_to_index( $this->add_coords( $coords, $this->get_add_coords( $choice, 1 ) ) ) ]->get_mainvalue();
      }
    }

    return false; // this never happens
  }

  protected function rule_2( $coords, $rule_counter = 0 ) {
    $choice = $this->make_binary( $rule_counter, $this->dimension );
    $others = false;
    foreach ( $choice as $key => $value ) {
      if ( $value == 1 && $coords[ $key ] == 0 ) {
        return false;
      } elseif ( $value == 0 && $coords[ $key ] > 0 ) {
        $others = true;
      }
    }
    if ( ! $others ) {
      return false;
    }

    $add_coords = $this->get_add_coords( $choice, 2 );

    $prev = $this->freespace[ $this->coords_to_index( $this->add_coords( $coords, $add_coords ) ) ]->get_boolvalue_at( $this->get_rule_index( $rule_counter, 2 ) );
    if ( $prev ) {
      return true;
    }

    //TODO: Zugriff auf Regel 4 (mehrfach möglich)
    
    return false; // this never happens
  }

  protected function rule_3( $coords, $rule_counter = 0 ) {
    $index_to_check = $this->dimension - 1 - $rule_counter;
    if ( $coords[ $index_to_check ] == 0 || $this->has_coord_zero( $coords, 0, $this->dimension ) ) {
      return false;
    }

    $current_point = $this->trajectories[ $index_to_check ]->get_point( $coords[ $index_to_check ] - 1 );
    $boolvalue = true;
    for ( $x = 0; $x < $this->dimension; $rule_counter++ ) {
      if ( $coords[ $x + $this->dimension ] == 0 ) {
        $boolvalue = false; // return true or false?
        break;
      }
      $point = $this->trajectories[ $x ]->get_point( $coords[ $x + $this->dimension ] - 1 );
      if ( $this->calc_distance( $point->get_pos(), $current_point->get_pos() ) > $epsilon ) {
        $boolvalue = false;
        break;
      }
    }
    if ( ! $boolvalue ) {
      return $boolvalue;
    }

    $add_coords = $this->get_add_coords( $this->make_binary( 0, $this->dimension ), 3 );
    if ( $this->freespace[ $this->coords_to_index( $this->add_coords( $coords, $add_coords ) ) ]->get_boolvalue_at( $this->get_rule_index( 0, 1 ) ) ) {
      return $boolvalue;
    }

    $boolvalue = false;
    $choices = $this->get_binary_choices( 0, intval( pow( 2, $this->dimension ) - 1 ), $this->dimension );
    for ( $choices as $choice ) {
      $add_coords = $this->get_add_coords( $choice, 3 );
      if ( $this->freespace[ $this->coords_to_index( $this->add_coords( $coords, $add_coords ) ) ]->get_boolvalue_at( $this->get_rule_index( $rule_counter, 3 ) ) ) {
        $boolvalue = true;
        break;
      }
    }

    return $boolvalue;
  }

  protected function fill_free_space() {
    $epsilon = 5.0; //TODO: how does it work without Epsilon?

    for ( $i = 0; $i < $this->freespace_size; $i++ ) {
      $coords = $this->index_to_coords( $i );
      $this->freespace[ $i ] = new \Studienprojekt\OrderedCase\BoolspacePoint( $coords );
      $boolvalues = array();

      // Fall A (1 boolvalue) --> Regel 1
      $boolvalues[] = $this->rule_1( $coords );

      // Fall B/D (2^k - 2 boolvalues) --> Regel 2
      for ( $j = 1; $j <= intval( pow( 2, $this->dimension ) - 2 ); $j++ ) {
        $boolvalues[] = $this->rule_2( $coords, $j );
      }

      // Fall C/E (k boolvalues) --> Regel 3
      for ( $j = 0; $j < $this->dimension; $j++ ) {
        $boolvalues[] = $this->rule_3( $coords, $j );
      }

      // Fall F/G (k * (2^(k-1) - 1) boolvalues) --> Regel 4
      for ( $j = 0; $j < $this->dimension; $j++ ) {
        $choices = $this->get_binary_choices( 1, intval( pow( 2, $this->dimension - 1 ) ), $this->dimension - 1 );
        foreach ( $choices as $choice ) {
          if ( $this->has_coord_zero( $coords ) ) {
            $boolvalues[] = false;
          } else {
            $index_to_check = $this->dimension - 1 - $j;
            $current_point = $this->trajectories[ $index_to_check ]->get_point( $coords[ $index_to_check ] - 1 );
            $boolvalue = true;
            for ( $x = 0; $x < $this->dimension; $j++ ) {
              if ( $coords[ $x + $this->dimension ] == 0 ) {
                $boolvalue = false; // return true or false?
                break;
              }
              $point = $this->trajectories[ $x ]->get_point( $coords[ $x + $this->dimension ] - 1 );
              if ( $this->calc_distance( $point->get_pos(), $current_point->get_pos() ) > $epsilon ) {
                $boolvalue = false;
                break;
              }
            }
            if ( ! $boolvalue ) {
              $boolvalues[] = $boolvalue;
            } else {
              //TODO: how does this rule work?
              //$boolvalues[] = false;
            }
          }
        }
      }

      //$this->freespace[ $i ]->set_boolvalues( $boolvalues );
    }
  }

  protected function get_rule_indices( $rule_index, $rule = 0 ) {
    if ( $rule < 1 || $rule > 4 ) {
      return array();
    }

    if ( $rule == 1 ) {
      return array( 0 );
    }

    $index = 1;

    if ( $rule > 2 ) {

    }
  }

  //TODO: figure out how to make get_rule_index() work
  protected function get_rule_index( $rule_counter, $rule = 0 ) {
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
      $index += $this->dimension;
    }

    return $index + $rule_counter;
  }

  protected function has_coord_zero( $coords, $limit = 0, $offset = 0 ) {
    if ( $limit == 0 ) {
      $limit = count( $coords );
    }
    for ( $i = $offset; $i < $limit; $i++ ) {
      if ( $coords[ $i ] == 0 ) {
        return true;
      }
    }
    return false;
  }

  protected function get_add_coords( $binary_choice, $rule = 0 ) {
    $add_coords = array();
    switch ( $rule ) {
      case 4:
        break;
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

  protected function get_boundary_coords( $binary_choice, $rule = 0 ) {
    $min_coords = array();
    $max_coords = array();
    switch ( $rule ) {
      case 4:
        break;
      case 3:
        break;
      case 2:
      case 1:
        for ( $i = 0; $i < $this->dimension; $i++ ) {
          if ( $binary_choice[ $i ] == 1 ) {
            $max_coords[] = count( $this->trajectories[ $i ] );
          } else {
            $max_coords[] = 0;
          }
          $min_coords[] = 0;
        }
        for ( $i = 0; $i < $this->dimension; $i++ ) {
          $max_coords[] = count( $this->trajectories[ $i ] );
          $min_coords[] = 0;
        }
        break;
      default:
        for ( $i = 0; $i < $this->dimension; $i++ ) {
          $max_coords[] = count( $this->trajectories[ $i ] );
          $min_coords[] = 0;
        }
        for ( $i = 0; $i < $this->dimension; $i++ ) {
          $max_coords[] = count( $this->trajectories[ $i ] );
          $min_coords[] = 0;
        }
    }
    return array( $min_coords, $max_coords );
  }

  protected function calc_distance( $pos1, $pos2 ) {
    return sqrt( pow( $pos1[0] - $pos2[0], 2 ) + pow( $pos1[1] - $pos2[1], 2 ) );
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
