<?php
/**
 * @file
 * Function: display_messages()
 */

namespace QTranslate\Client;

/**
 * Display any messages, warnings and errors.
 *
 * @param array $messages
 *   Array of messages, usually returned from qtr rest services.
 *   Each item is an array that contains a verse message and a type,
 *   where type can be one of: status, warning, error.
 */
function display_messages($messages) {
  foreach ($messages as $message) {
    list($msg, $type) = $message;
    if (!in_array($type, array('status', 'warning', 'error'))) {
      $type = 'status';
    }
    drupal_set_message($msg, $type);
  }
}
