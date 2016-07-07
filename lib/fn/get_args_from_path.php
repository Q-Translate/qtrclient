<?php
/**
 * @file
 * Function: get_args_from_path()
 */

namespace QTranslate\Client;
use \qcl;

/**
 * Extract the $chapter, $verse and $lng from the current path,
 * if it is in the form: /qtr/$lng/$chapter/$verse
 */
function get_args_from_path() {
  // Get the arguments from the path.
  $languages = qcl::get_languages();
  $args = explode('/', current_path());
  $lng = $args[1];
  if ($args[0]=='qtr' and isset($languages[$lng])) {
    $chapter = $args[2];
    $verse = $args[3];
  }
  else {
    $lng = $chapter = $verse = NULL;
  }

  return [$lng, $chapter, $verse];
}
