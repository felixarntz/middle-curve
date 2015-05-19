<?php

namespace Studienprojekt;

class App {
  private $trajectories = array();
  private $mode = '';

  public function __construct( $trajectories, $mode ) {
    if ( ! is_array( $trajectories ) || count( $trajectories ) < 2 ) {
      return false;
    }

    foreach ( $trajectories as $key => $trajectory ) {
      $points = isset( $trajectory['points'] ) ? $trajectory['points'] : array();
      $name = isset( $trajectory['name'] ) ? $trajectory['name'] : 'DEFAULT';

      $this->trajectories[] = new \Studienprojekt\Base\Trajectory( $points, $name, $key );
    }

    $this->mode = $mode;
  }

  public function calculate() {
    $results = array();

    $algorithm = null;

    switch ( $this->mode ) {
      case 'restricted':
        break;
      case 'ordered':
        $algorithm = new \Studienprojekt\OrderedCase\Algorithm( $this->trajectories );
        break;
      case 'unordered':
        $algorithm = new \Studienprojekt\UnorderedCase\Algorithm( $this->trajectories );
        break;
      case 'discrete':
      default:
        $algorithm = new \Studienprojekt\DiscreteFrechet\Algorithm( $this->trajectories );
        break;
    }

    if ( $algorithm !== null ) {
      $algorithm->run();
      $results = $algorithm->get_results();
    }

    return $results;
  }
}
