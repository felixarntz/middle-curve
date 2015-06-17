<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Middle Curve</title>
    <meta name="robots" content="noindex,nofollow">
    <link rel="stylesheet" href="./assets/css/bootstrap.min.css">
  </head>

  <body>
    <header class="jumbotron">
      <div class="container">
        <h1>Calculating a Middle Curve</h1>
        <p>Add some trajectories below to get started!</p>
      </div>
    </header>

    <div class="container">

      <main class="row">
        <div class="col-md-12">
          <form action="" method="post">
            <div id="trajectory-container">
              <p class="text-right">
                <button type="button" class="btn btn-info btn-lg trajectory-button">Add Trajectory</button>
              </p>
            </div>
            <button type="submit" class="btn btn-primary btn-lg submit-button" disabled>Submit</button>
            <select name="mode" class="form-control input-lg" style="display:inline-block;width:30%;">
              <option value="discrete">Discrete Frechet</option>
              <option value="unordered" selected>Unordered Case</option>
              <option value="ordered">Ordered Case</option>
            </select>
          </form>
          <p id="calculations-loader" class="text-center" style="display:none;">Calculations loading...</p>
          <div id="calculations"></div>
        </div>
      </main>

      <hr>

      <footer>
        <p>&copy; 2015 RUB - AI - Studienprojekt 9b</p>
      </footer>

    </div>

    <div class="modal fade" id="notice" tabindex="-1" role="dialog" aria-labelledby="noticeLabel" aria-hidden="true">
      <div class="modal-dialog">
        <div class="modal-content">
          <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
            <h4 class="modal-title" id="noticeLabel"><span class="glyphicon glyphicon-warning-sign" aria-hidden="true"></span> Error</h4>
          </div>
          <div class="modal-body">
            Please fill in all the required fields (every field except the "Time" fields).
          </div>
          <div class="modal-footer">
            <button type="button" class="btn btn-primary" data-dismiss="modal">OK</button>
          </div>
        </div>
      </div>
    </div>

    <script id="trajectory-template" type="text/template">
      <div id="trajectory-{{key}}" data-key="{{key}}" class="trajectory well">
        <div class="row">
          <div class="col-md-8 col-sm-8 col-xs-7">
            <div class="form-group">
              <input type="text" name="trajectories[{{key}}][name]" class="form-control input-lg" placeholder="Trajectory Name" required>
            </div>
          </div>
          <div class="col-md-4 col-sm-4 col-xs-5">
            <button type="button" class="btn btn-info btn-lg btn-block point-button">Add Point</button>
          </div>
        </div>
      </div>
    </script>

    <script id="point-template" type="text/template">
      <hr>
      <div id="point-{{key}}-{{id}}" class="point form-inline">
        <div class="form-group">
          <label>Point {{number}}:</label>
          <input type="number" name="trajectories[{{key}}][points][{{id}}][x]" class="form-control" placeholder="X coordinate" step="0.25" required>
          <input type="number" name="trajectories[{{key}}][points][{{id}}][y]" class="form-control" placeholder="Y coordinate" step="0.25" required>
          <input type="text" name="trajectories[{{key}}][points][{{id}}][time]" class="form-control" placeholder="Time">
        </div>
      </div>
    </script>

    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.2/jquery.min.js"></script>
    <script src="./assets/js/bootstrap.min.js"></script>
    <script src="./assets/js/app.js"></script>
  </body>
</html>
