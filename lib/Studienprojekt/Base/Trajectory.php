<?php

namespace Studienprojekt\Base;

class Trajectory {
  private $index = -1;
  private $name = '';
  private $points = array();

  public function __construct( $points, $name, $index ) {
    if ( ! is_array( $points ) || count( $points ) < 2 ) {
      return false;
    }

    $this->name = $name;

    $this->index = abs( intval( $index ) );

    foreach ( $points as $key => $point ) {
      $pos = $point;
      $time = '';
      if ( isset( $pos['time'] ) ) {
        $time = $pos['time'];
        unset( $pos['time'] );
      }
      $this->points[] = new \Studienprojekt\Base\Point( $pos, $time, $key, $this->index );
    }
  }

  public function get_name() {
    return $this->name;
  }

  public function get_point( $index ) {
    if ( isset( $this->points[ $index ] ) ) {
      return $this->points[ $index ];
    }
    return null;
  }

  public function get_points() {
    return $this->points;
  }

  public function get_index() {
    return $this->index;
  }

  public function get_length() {
    return count( $this->points );
  }
}
