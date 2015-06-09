<?php

namespace Studienprojekt\DiscreteFrechet;

class FreespacePoint {
  protected $indices = array();
  protected $center_distance = 100000000.0;
  protected $center_point = null;

  protected $next = -1;
  protected $cost = 100000000.0;
  protected $visited = false;

  public function __construct( $indices ) {
    $this->indices = $indices;
  }

  public function get_indices() {
    return $this->indices;
  }

  public function set_center_point( $center_point, $center_distance ) {
    if ( $center_distance < $this->center_distance ) {
      $this->center_point = $center_point;
      $this->center_distance = $center_distance;
    }
  }

  public function get_center_distance() {
    return $this->center_distance;
  }

  public function get_center_point() {
    return $this->center_point;
  }

  public function set_next( $index, $cost ) {
    if ( $this->center_distance + $cost < $this->cost ) {
      $this->next = $index;
      $this->cost = $this->center_distance + $cost;
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
    return count( $this->indices );
  }
}
