<?php

namespace Studienprojekt\OrderedCase;

class BoolspacePoint {
  protected $indices = array();
  protected $dimension = 0;
  protected $mainvalue = false;
  protected $values = array();

  public function __construct( $indices, $infinite ) {
    $this->indices = $indices;
    $this->dimension = intval( count( $this->indices ) / 2 );
    $this->mainvalue = $infinite;
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

  public function get_indices() {
    return $this->indices;
  }

  public function get_dimension() {
    return $this->dimension;
  }
}
