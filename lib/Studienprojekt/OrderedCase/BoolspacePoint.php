<?php

namespace Studienprojekt\OrderedCase;

class BoolspacePoint {
  protected $indices = array();
  protected $dimension = 0;
  protected $mainvalue = false;
  protected $boolvalues = array();

  public function __construct( $indices ) {
    $this->indices = $indices;
    $this->dimension = intval( count( $this->indices ) / 2 );
  }

  public function set_boolvalues( $boolvalues ) {
    $this->boolvalues = $boolvalues;
    $this->mainvalue = false;
    foreach ( $this->boolvalues as $boolvalue ) {
      if ( $boolvalue ) {
        $this->mainvalue = true;
        break;
      }
    }
  }

  public function get_boolvalues() {
    return $this->boolvalues;
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
