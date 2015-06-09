<?php

namespace Studienprojekt\OrderedCase;

class BoolspacePoint {
  protected $indices = array();
  protected $dimension = 0;
  protected $mainvalue = 0.0;
  protected $values = array();

  protected $next = -1;
  protected $cost = 1000000000000000000.0;
  protected $visited = false;

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

  public function set_next( $index, $cost ) {
    if ( $this->mainvalue + $cost < $this->cost ) {
      $this->next = $index;
      $this->cost = $this->mainvalue + $cost;
    }
    $this->visited = true;
  }

  public function get_next() {
    return $this->next;
  }

  public function get_cost() {
    return $this->cost;
  }

  public function get_visited() {
    return $this->visited;
  }

  public function get_dimension() {
    return $this->dimension;
  }
}
