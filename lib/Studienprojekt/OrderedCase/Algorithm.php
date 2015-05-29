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
    $epsilon = 5.0; //TODO: how does it work without Epsilon?

    for ( $i = 0; $i < $this->freespace_size; $i++ ) {
      $coords = $this->index_to_coords( $i );
      $this->freespace[ $i ] = new \Studienprojekt\OrderedCase\BoolspacePoint( $coords );
      $boolvalues = array();

      // Fall A (1 boolvalue)
      if ( $i == 0 ) {
        $boolvalues[] = true;
      } else {
        $choice = $this->make_binary( 0, $this->dimension );
        $boundary_coords = $this->get_boundary_coords( $choice, 1 );
        if ( $this->is_in_boundaries( $coords, $boundary_coords[0], $boundary_coords[1] ) ) {
          $boolvalues[] = false;
        } else {
          $choices = $this->get_binary_choices( 1, intval( pow( 2, $this->dimension ) ), $this->dimension );
          foreach ( $choices as $choice ) {
            $boundary_coords = $this->get_boundary_coords( $choice, 1 );
            if ( $this->is_in_boundaries( $coords, $boundary_coords[0], $boundary_coords[1] ) ) {
              $add_coords = $this->get_add_coords( $choice, 1 );
              $boolvalues[] = $this->freespace[ $this->coords_to_index( $this->add_coords( $coords, $add_coords ) ) ]->get_mainvalue();
              break;
            }
          }
        }
      }

      // Fall B/D (2^k - 2 boolvalues)
      $choices = $this->get_binary_choices( 1, intval( pow( 2, $this->dimension ) - 1 ), $this->dimension );
      $rule_2_counter = 0;
      for ( $choices as $choice ) {
        if ( $this->has_coord_zero( $coords, $this->dimension ) ) {
          $boolvalues[] = false;
        } else {
          $add_coords = $this->get_add_coords( $choice, 2 );
          $boolvalues[] = $this->freespace[ $this->coords_to_index( $this->add_coords( $coords, $add_coords ) ) ]->get_boolvalue_at( $this->get_rule_index( $rule_2_counter, 4, $rule_2_counter ) ) || $this->freespace[ $this->coords_to_index( $this->add_coords( $coords, $add_coords ) ) ]->get_boolvalue_at( $this->get_rule_index( $rule_2_counter, 2 ) );
        }
        $rule_2_counter++;
      }

      // Fall C/E (k boolvalues)
      for ( $j = 0; $j < $this->dimension; $j++ ) {
        $index_to_check = $this->dimension - 1 - $j;
        if ( $coords[ $index_to_check ] == 0 || $this->has_coord_zero( $coords, 0, $this->dimension ) ) {
          $boolvalues[] = false;
        } else {
          $current_point = $this->trajectories[ $index_to_check ]->get_point( $coords[ $index_to_check ] );
          $boolvalue = true;
          for ( $x = 0; $x < $this->dimension; $j++ ) {
            $point = $this->trajectories[ $x ]->get_point( $coords[ $x + $this->dimension ] );
            if ( $this->calc_distance( $point->get_pos(), $current_point->get_pos() ) > $epsilon ) {
              $boolvalue = false;
              break;
            }
          }
          if ( ! $boolvalue ) {
            $boolvalues[] = $boolvalue;
          } else {
            $add_coords = $this->get_add_coords( $this->make_binary( 0, $this->dimension ), 3 );
            if ( $this->freespace[ $this->coords_to_index( $this->add_coords( $coords, $add_coords ) ) ]->get_boolvalue_at( $this->get_rule_index( 0, 1 ) ) ) {
              $boolvalues[] = $boolvalue;
            } else {
              $boolvalue = false;
              $choices = $this->get_binary_choices( 0, intval( pow( 2, $this->dimension ) - 1 ), $this->dimension );
              for ( $choices as $choice ) {
                $add_coords = $this->get_add_coords( $choice, 3 );
                if ( $this->freespace[ $this->coords_to_index( $this->add_coords( $coords, $add_coords ) ) ]->get_boolvalue_at( $this->get_rule_index( $j, 3 ) ) ) {
                  $boolvalue = true;
                  break;
                }
              }
              $boolvalues[] = $boolvalue;
            }
          }
        }
      }

      // Fall F/G (k * (2^(k-1) - 1) boolvalues)
      for ( $j = 0; $j < $this->dimension; $j++ ) {
        $choices = $this->get_binary_choices( 1, intval( pow( 2, $this->dimension - 1 ) ), $this->dimension - 1 );
        foreach ( $choices as $choice ) {
          if ( $this->has_coord_zero( $coords ) ) {
            $boolvalues[] = false;
          } else {
            $index_to_check = $this->dimension - 1 - $j;
            $current_point = $this->trajectories[ $index_to_check ]->get_point( $coords[ $index_to_check ] );
            $boolvalue = true;
            for ( $x = 0; $x < $this->dimension; $j++ ) {
              $point = $this->trajectories[ $x ]->get_point( $coords[ $x + $this->dimension ] );
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

  //TODO: figure out how to make get_rule_index() work
  protected function get_rule_index( $rule_counter, $rule = 0, $offset = 0 ) {
    if ( $rule < 1 || $rule > 4 ) {
      return -1;
    }

    if ( $rule == 1 ) {
      return 0;
    }

    $index = 1;

    if ( $rule > 2 ) {
      $index += pow( 2, $this->dimension ) - 2;
    }
    if ( $rule > 3 ) {
      $index += $this->dimension;
      $index += $offset * ( pow( 2, $this->dimension - 1 ) - 1 );
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
            $max_coords[] = count( $this->trajectories[ $i ] ) - 1;
          } else {
            $max_coords[] = 0;
          }
          $min_coords[] = 0;
        }
        for ( $i = 0; $i < $this->dimension; $i++ ) {
          $max_coords[] = count( $this->trajectories[ $i ] ) - 1;
          $min_coords[] = 0;
        }
        break;
      default:
        for ( $i = 0; $i < $this->dimension; $i++ ) {
          $max_coords[] = count( $this->trajectories[ $i ] ) - 1;
          $min_coords[] = 0;
        }
        for ( $i = 0; $i < $this->dimension; $i++ ) {
          $max_coords[] = count( $this->trajectories[ $i ] ) - 1;
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
}
