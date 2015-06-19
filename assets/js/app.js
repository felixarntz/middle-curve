// source: https://github.com/cezary/JSONForms
var Gn={"&":"&amp;","<":"&lt;",">":"&gt;",'"':"&quot;","'":"&#x27;"}

function isNumeric(value) {
  return /^\d+$/.test(value);
}

function parsePath(path) {
  var originalPath = path;
  var steps = [];
  var error = false;
  var firstKey = path.substr(0, path.indexOf('['));
  if (!firstKey.length) {
    error = true;
  } else {
    path = path.substr(path.indexOf('['), path.length);
    steps.push({
      key: firstKey,
      last: !path.length,
      type:'object'
    });
  }

  var key;
  key = path.substr(1, path.indexOf(']')-1);

  while (path.length && !error) {
    if (path[0] === '[' && path[1] === ']') {
      steps.push({
        append: true,
        type: 'array'
      });
      path = path.substr(2, path.length);
      error = path.length !== 0;
    } else if (isNumeric(key = path.substr(1, path.indexOf(']')-1))) {
      key = parseInt(key, 10);
      path = path.substr(path.indexOf(']')+1, path.length);
      steps.push({
        key: key,
        type: 'array'
      })
    } else if ((key = path.substr(1, path.indexOf(']')-1)) && key.indexOf('[') === -1) {
      path = path.substr(path.indexOf(']')+1, path.length);
      steps.push({
        key: key,
        type: 'object'
      });
    } else {
      error = true;
    }
  }

  if (error) {
    steps = [{
      key: originalPath,
      last: true,
      type: 'object'
    }];
  } else {
    for (var i = 0; i < steps.length; i++) {
      var step = steps[i];
      var nextStep = steps[i+1];
      if (nextStep) {
        step.nextType = nextStep.type;
      } else {
        step.last = true;
      }
    }
  }

  return steps;
}

function setValue(context, step, currentValue, entryValue, isFile) {
  if (isFile) {
    entryValue = {
      name: 'filename',
      type: 'filetype',
      body: 'filebody'
    }
  }
  if (step.last) {
    if (typeof currentValue === 'undefined') {
      if (step.append) {
        context.push(entryValue);
      } else {
        context[step.key] = entryValue;
      }
    } else if (currentValue.constructor == Array) {
      context[step.key].push(entryValue);
    } else if (currentValue.constructor == Object && !isFile) {
      return setValue(currentValue, {key:'', last:true, type:'object'}, currentValue[''], entryValue, isFile);

    } else {
      context[step.key] = [currentValue, entryValue];
    }
    return context;
  }

  if (typeof currentValue === 'undefined') {
    if (step.nextType === 'array') {
      context[step.key] = [];
    } else {
      context[step.key] = {};
    }
    return context[step.key];
  } else if (currentValue.constructor === Object) {
    return context[step.key];
  } else if (currentValue.constructor === Array) {
    if (step.nextType === 'array') {
      return currentValue;
    } else {
      var object = {};
      currentValue.forEach(function(item, i) {
        if (typeof item !== 'undefined') {
          object[i] = item;
        } else {
          context[step.key] = object;
        }
      });
      return object;
    }
  } else {
    var object = {'': currentValue};
    context[step.key] = object;
    return object;
  }
}

function JSONEncode(formEl) {
  var entries = collectEntries(formEl);
  var resultingObject = {};

  entries.forEach(function(entry) {
    var isFile = entry.value && entry.value.body !== undefined;
    var steps = parsePath(entry.name);
    var context = resultingObject;
    for (var i = 0; i < steps.length; i++) {
      var step = steps[i];
      var currentValue = context[step.key];
      context = setValue(context, step, currentValue, entry.value, isFile);
    }
  });

  return resultingObject;
}

function collectEntries(formEl) {
  return []
    // input elements
    .concat(Array.prototype.slice.call(formEl.querySelectorAll('input:not([type=submit])')).map(function(el) {
      var entry = { name: el.name, value: el.value };

      switch (el.type) {
        case 'checkbox':
          entry.value = el.checked
          break;
        case 'number':
          entry.value = parseFloat(el.value);
          break;
        case 'radio':
          if (el.checked)
            entry.value === el.value;
          else
            return null;
          break;
      }
      return entry;
    }))
    // select elements
    .concat(Array.prototype.slice.call(formEl.querySelectorAll('select:not([multiple])')).map(function(el) {
      return { name: el.name, value: el.value };
    }))
    .concat(Array.prototype.slice.call(formEl.querySelectorAll('select[multiple] option[selected]')).map(function(el) {
      var selectEl = parent(el, 'select');
      return { name: selectEl.name, value: el.value };
    }))
    .concat(Array.prototype.slice.call(formEl.querySelectorAll('textarea')).map(function(el) {
      return { name: el.name, value: el.value };
    }))
    .filter(function(entry) { return entry;});
};

function parent(el, tagName) {
  tagName = tagName.toLowerCase();

  while (el && el.parentNode) {
    el = el.parentNode;
    if (el.tagName && el.tagName.toLowerCase() == tagName) {
      return el;
    }
  }
  return null;
}

// Middle Curve app

jQuery( document ).ready(function($) {

  var ajax_url = window.location.href;
  if ( ajax_url.indexOf( '.php' ) > -1 ) {
    var parts = ajax_url.split( '/' );
    parts[ parts.length - 1 ] = 'calculate.php';
    ajax_url = parts.join( '/' );
  } else {
    if ( ajax_url.substring( ajax_url.length - 1 ) != '/' ) {
      ajax_url += '/';
    }
    ajax_url += 'calculate.php';
  }
  var trajectory_template = $( '#trajectory-template' ).text();
  var point_template = $( '#point-template' ).text();

  $( '#notice' ).modal({
    keyboard: false,
    show: false
  });

  function check_disabled_state() {
    var disabled = false;
    if ( $( '.trajectory' ).length < 2 ) {
      disabled = true;
    } else {
      $( '.trajectory' ).each(function() {
        if ( $( this ).find( '.point' ).length < 2 ) {
          disabled = true;
        }
      });
    }
    $( '.submit-button' ).prop( 'disabled', disabled );
  }

  function parse_template( template, data ) {
    return template.replace( /\{\{([A-Za-z_]+)\}\}/g, function( match, content, offset ) {
      if ( typeof data[ content ] !== 'undefined' ) {
        return data[ content ];
      }
      return match;
    });
  }

  $( document ).on( 'click', '.trajectory-button', function( e ) {
    var data = {
      key: $( '.trajectory' ).length
    };
    var content = parse_template( trajectory_template, data );

    $( '#trajectory-container' ).append( content );

    check_disabled_state();
  });

  $( document ).on( 'click', '.point-button', function( e ) {
    var $trajectory = $( this ).parents( '.trajectory' );
    var data = {
      key: $trajectory.data( 'key' ),
      id: $trajectory.find( '.point' ).length,
      number: $trajectory.find( '.point' ).length + 1
    };
    var content = parse_template( point_template, data );

    $trajectory.append( content );

    check_disabled_state();
  });

  $( document ).on( 'click', '.submit-button', function( e ) {
    var $form = $( this ).parents( 'form' );
    var filled = true;
    $form.find( '[required]' ).each(function() {
      if ( $( this ).val() == '' ) {
        filled = false;
      }
    });

    if ( filled ) {
      var post_data = JSONEncode( $form.get( 0 ) );
      var mode = typeof post_data.mode === 'string' ? post_data.mode : 'unordered';
      $( '#calculations-loader' ).show();
      $.ajax({
        url: ajax_url,
        method: 'POST',
        data: post_data,
        dataType: 'json',
        error: function( xhr, statusCode, statusText ) {
          $( '#calculations-loader' ).hide();
          $( '#calculations' ).html( '<p>' + statusCode + ' - ' + statusText + '</p>' );
        },
        success: function( response, statusCode, xhr ) {
          $( '#calculations-loader' ).hide();
          if ( typeof response.error === 'string' && response.error.length > 0 ) {
            $( '#calculations' ).html( '<p>' + response.error + '</p>' );
          } else {
            var data = response.data;
            var output = '<hr><div class="well well-lg"><h2 class="text-center">Results</h2>';
            var points_output = [];
            switch ( mode ) {
              case 'restricted':
                output += '<p class="lead">Calculations not yet implemented.</p>';
                break;
              case 'ordered':
                output += '<h3>Path</h3>';
                output += '<p class="lead">Middle Curve:';
                for ( var p in data.middle_curve ) {
                  output += ' &rarr; <strong>' + data.middle_curve[ p ].trajectory_name + '<sub>' + ( data.middle_curve[ p ].index + 1 ) + '</sub></strong>';
                }
                output += '</p>';
                output += '<p class="lead">Epsilon: <strong>' + data.epsilon + '</strong></p>';
                output += '<table class="table table-bordered table-hover table-responsive">';
                output += '<thead><tr><th>#</th><th>Boolspace Point</th><th>Center Point</th><th>Center Point Coords</th><th>Mainvalue</th></tr></thead>';
                output += '<tbody>';
                for ( var f in data.path ) {
                  output += '<tr><td>' + ( parseInt( f ) + 1 ) + '</td><td>(' + data.path[ f ].coords.join( ', ' ) + ')</td><td>' + data.path[ f ].center_point.trajectory_name + '<sub>' + ( data.path[ f ].center_point.index + 1 ) + '</sub></td><td>(' + data.path[ f ].center_point.pos.x + '|' + data.path[ f ].center_point.pos.y + ')</td><td>' + data.path[ f ].mainvalue + '</td></tr>';
                }
                output += '</tbody>';
                output += '</table>';
                var cover_points_output = [];
                var values_output = [];
                output += '<h3>Boolspace</h3>';
                output += '<table class="table table-bordered table-hover table-responsive">';
                output += '<thead><tr><th>#</th><th>Boolspace Point</th><th>Values</th><th>Trajectory Points</th><th>Cover Points</th></tr></thead>';
                output += '<tbody>';
                for ( var i in data.boolspace ) {
                  points_output = [];
                  for ( var p in data.boolspace[ i ].points ) {
                    points_output.push( data.boolspace[ i ].points[ p ].trajectory_name + '<sub>' + ( data.boolspace[ i ].points[ p ].index + 1 ) + '</sub> (' + data.boolspace[ i ].points[ p ].pos.x + '|' + data.boolspace[ i ].points[ p ].pos.y + ')' );
                  }
                  cover_points_output = [];
                  for ( var p in data.boolspace[ i ].cover_points ) {
                    cover_points_output.push( data.boolspace[ i ].cover_points[ p ].trajectory_name + '<sub>' + ( data.boolspace[ i ].cover_points[ p ].index + 1 ) + '</sub> (' + data.boolspace[ i ].cover_points[ p ].pos.x + '|' + data.boolspace[ i ].cover_points[ p ].pos.y + ')' );
                  }
                  values_output = [];
                  for ( var v in data.boolspace[ i ].values ) {
                    values_output.push( data.boolspace[ i ].values[ v ] );
                  }
                  output += '<tr><td>' + ( parseInt( i ) + 1 ) + '</td><td>(' + data.boolspace[ i ].coords.join( ', ' ) + ')</td><td>' + values_output.join( ', ' ) + '</td><td>' + points_output.join( ', ' ) + '</td><td>' + cover_points_output.join( ', ' ) + '</td></tr>';
                }
                output += '</tbody>';
                output += '</table>';
                break;
              case 'unordered':
              case 'discrete':
              default:
                output += '<h3>Path</h3>';
                output += '<p class="lead">Middle Curve:';
                for ( var p in data.middle_curve ) {
                  output += ' &rarr; <strong>' + data.middle_curve[ p ].trajectory_name + '<sub>' + ( data.middle_curve[ p ].index + 1 ) + '</sub></strong>';
                }
                output += '</p>';
                output += '<p class="lead">Epsilon: <strong>' + data.epsilon + '</strong></p>';
                output += '<table class="table table-bordered table-hover table-responsive">';
                output += '<thead><tr><th>#</th><th>Freespace Point</th><th>Center Point</th><th>Center Point Coords</th><th>Center Distance</th></tr></thead>';
                output += '<tbody>';
                for ( var f in data.path ) {
                  output += '<tr><td>' + ( parseInt( f ) + 1 ) + '</td><td>(' + data.path[ f ].coords.join( ', ' ) + ')</td><td>' + data.path[ f ].center_point.trajectory_name + '<sub>' + ( data.path[ f ].center_point.index + 1 ) + '</sub></td><td>(' + data.path[ f ].center_point.pos.x + '|' + data.path[ f ].center_point.pos.y + ')</td><td>' + data.path[ f ].center_distance + '</td></tr>';
                }
                output += '</tbody>';
                output += '</table>';
                output += '<h3>Free Space</h3>';
                output += '<table class="table table-bordered table-hover table-responsive">';
                output += '<thead><tr><th>#</th><th>Freespace Point</th><th>Center Point</th><th>Center Point Coords</th><th>Center Distance</th><th>Trajectory Points</th></tr></thead>';
                output += '<tbody>';
                for ( var i in data.freespace ) {
                  points_output = [];
                  for ( var p in data.freespace[ i ].points ) {
                    points_output.push( data.freespace[ i ].points[ p ].trajectory_name + '<sub>' + ( data.freespace[ i ].points[ p ].index + 1 ) + '</sub> (' + data.freespace[ i ].points[ p ].pos.x + '|' + data.freespace[ i ].points[ p ].pos.y + ')' );
                  }
                  output += '<tr><td>' + ( parseInt( i ) + 1 ) + '</td><td>(' + data.freespace[ i ].coords.join( ', ' ) + ')</td><td>' + data.freespace[ i ].center_point.trajectory_name + '<sub>' + ( data.freespace[ i ].center_point.index + 1 ) + '</sub></td><td>(' + data.freespace[ i ].center_point.pos.x + '|' + data.freespace[ i ].center_point.pos.y + ')</td><td>' + data.freespace[ i ].center_distance + '</td><td>' + points_output.join( ', ' ) + '</td></tr>';
                }
                output += '</tbody>';
                output += '</table>';
                break;
            }
            output += '</div>';
            $( '#calculations' ).html( output );
          }
        }
      });
    } else {
      $( '#notice' ).modal( 'show' );
    }
    e.preventDefault();
  });

});
