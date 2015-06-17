<?php

namespace Studienprojekt\OrderedCase;

class BoolspacePoint {
  protected $indices = array();
  protected $dimension = 0;
  protected $mainvalue = 0.0;
  protected $values = array();

  protected $center_point = null;
  protected $previous = -1;

  public function __construct( $indices, $infinite ) {
    $this->indices = $indices;
    $this->dimension = intval( count( $this->indices ) / 2 );
    $this->mainvalue = $infinite;
  }

  public function get_indices() {
    return $this->indices;
  }

  public function set_values( $values ) {
    $this->values = $values;
    $this->mainvalue = min( $this->values );
  }

  public function get_values() {
    return $this->values;
  }

  public function get_value_at( $i ) {
    if ( isset( $this->values[ $i ] ) ) {
      return $this->values[ $i ];
    }
  }

  public function get_mainvalue() {
    return $this->mainvalue;
  }

  public function set_center_point( $center_point, $distance ) {
    if ( $distance < $this->mainvalue ) {
      $this->mainvalue = $distance;
      $this->center_point = $center_point;
    }
  }

  public function get_center_point() {
    return $this->center_point;
  }

  public function set_previous( $previous ) {
    if ( $previous > -1 ) {
      if ( $this->previous == -1 || $previous < $this->previous ) {
        $this->previous = $previous;
      }
    }
  }

  public function get_previous() {
    return $this->previous;
  }

  public function get_dimension() {
    return $this->dimension;
  }
}
