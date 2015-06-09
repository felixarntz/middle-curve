<?php

http_response_code( 200 );
header( 'Content-Type: text/json' );

$response = array(
  'data'  => array(),
  'error' => '',
);

ini_set( 'log_errors', '1' );
ini_set( 'error_log', dirname( __FILE__ ) . '/errors.log' );

if ( isset( $_POST ) && isset( $_POST['trajectories'] ) ) {
  $trajectories = $_POST['trajectories'];
  $mode = isset( $_POST['mode'] ) ? $_POST['mode'] : 'discrete';

  $current_dir = dirname( __FILE__ );

  require_once $current_dir . '/lib/Studienprojekt/App.php';
  require_once $current_dir . '/lib/Studienprojekt/Base/Trajectory.php';
  require_once $current_dir . '/lib/Studienprojekt/Base/Point.php';
  require_once $current_dir . '/lib/Studienprojekt/Base/Algorithm.php';
  require_once $current_dir . '/lib/Studienprojekt/DiscreteFrechet/Algorithm.php';
  require_once $current_dir . '/lib/Studienprojekt/DiscreteFrechet/FreespacePoint.php';
  require_once $current_dir . '/lib/Studienprojekt/UnorderedCase/Algorithm.php';
  require_once $current_dir . '/lib/Studienprojekt/UnorderedCase/FreespacePoint.php';
  require_once $current_dir . '/lib/Studienprojekt/OrderedCase/Algorithm.php';
  require_once $current_dir . '/lib/Studienprojekt/OrderedCase/BoolspacePoint.php';

  $app = new Studienprojekt\App( $trajectories, $mode );

  if ( $app ) {
    $response['data'] = $app->calculate();
  }
} else {
  $response['error'] = 'No trajectory data specified.';
}

echo json_encode( $response );
die();
