<?php

namespace Studienprojekt\RestrictedCase;

class XspacePoint {
  protected $indices = array();
  protected $dimension = 0;

  protected $value = false;
  protected $center_point = null;
  protected $previous = -1;

  public function __construct( $indices, $infinite ) {
    $this->indices = $indices;
    $this->dimension = count( $this->indices );
  }

  public function enable( $center_point ) {
    $this->center_point = $center_point;
    $this->value = true;
  }

  public function set_previous( $previous ) {
    $this->previous = $previous;
  }

  public function get_indices() {
    return $this->indices;
  }

  public function get_value() {
    return $this->value;
  }

  public function get_center_point() {
    return $this->center_point;
  }

  public function get_previous() {
    return $this->previous;
  }

  public function get_dimension() {
    return $this->dimension;
  }
}
