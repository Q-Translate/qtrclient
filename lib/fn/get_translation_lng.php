<?php
/**
 * @file
 * Function: get_translation_lng()
 */

namespace BTranslator\Client;
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
    $lng = variable_get('qtrClient_translation_lng', 'fr');
    if ($lng == 'all')  $lng = 'fr';
  }

  return $lng;
}
