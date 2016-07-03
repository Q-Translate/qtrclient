<?php
/**
 * @file
 * Function: get_translation_lng()
 */

namespace QTranslate\Client;
use \qcl;

/**
 * Return the language of translations.
 */
function get_translation_lng() {
  if (oauth2_user_is_authenticated()) {
    $qtr_user = oauth2_user_get();
    $lng = $qtr_user['translation_lng'];
  }
  else {
    $lng = variable_get('qtrClient_translation_lng', 'en');
    if ($lng == 'all')  $lng = 'en';
  }

  return $lng;
}
