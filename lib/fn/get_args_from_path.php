<?php
/**
 * @file
 * Function: get_args_from_path()
 */

namespace QTranslate\Client;
use \qcl;

/**
 * Extract the $chapter, $verse and $lng from the current path,
 * if it is in the form: /qtr/$chapter/$verse/$lng
 */
function get_args_from_path() {
  // Get the arguments from the path.
  $args = explode('/', current_path());
  if ($args[0]=='qtr' and is_numeric($args[1])) {
    $chapter = $args[1];
    $verse = $args[2];
    $lng = $args[3];
  }
  else {
    $chapter = $verse = $lng = NULL;
  }

  return [$chapter, $verse, $lng];
}
