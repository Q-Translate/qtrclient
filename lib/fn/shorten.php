<?php
/**
 * @file
 * Function: shorten()
 */

namespace QTranslate\Client;

/**
 * Shorten the given verse.
 *
 * From the given (possibly long) verse, returns a short verse
 * of the given length that can be suitable for title, subject, etc.
 */
function shorten($verse, $length) {
  $str = str_replace("\n", ' ', $verse);
  $str = str_replace(' <==> ', ' (==) ', $str);
  $str = strip_tags($str);
  if (strlen($str) > $length) {
    $str = substr($str, 0, strrpos(substr($str, 0, $length - 3), ' '));
    $str .= '...';
  }
  $str = str_replace(' (==) ', ' <==> ', $str);
  return $str;
}
