<?php

namespace Studienprojekt\Base;

class Point {
  private $index = -1;
  private $trajectory_index = -1;
  private $pos = array();
  private $time = '';

  public function __construct( $pos, $time, $index, $trajectory_index ) {
    if ( ! is_array( $pos ) || count( $pos ) < 2 ) {
      return false;
    }

    $this->time = $time;

    $this->index = abs( intval( $index ) );
    $this->trajectory_index = abs( intval( $trajectory_index ) );
    
    if ( isset( $pos['x'] ) && isset( $pos['y'] ) ) {
      $this->pos[0] = $pos['x'];
      $this->pos[1] = $pos['y'];
    } elseif ( isset( $pos[0] ) && isset( $pos[1] ) ) {
      $this->pos[0] = $pos[0];
      $this->pos[1] = $pos[1];
    }
  }

  public function get_pos() {
    return $this->pos;
  }

  public function get_pos_x() {
    return $this->pos[0];
  }

  public function get_pos_y() {
    return $this->pos[1];
  }

  public function get_time() {
    return $this->time;
  }

  public function get_index() {
    return $this->index;
  }

  public function get_trajectory_index() {
    return $this->trajectory_index;
  }
}
