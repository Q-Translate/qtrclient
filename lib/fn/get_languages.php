<?php
/**
 * @file
 * Function: get_languages()
 */

namespace BTranslator\Client;
use \qcl;

/**
 * Return an array of languages and their details.
 */
function get_languages() {
  $cache = cache_get('languages', 'cache_qtrClient');
  // Return cache if possible.
  if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
    return $cache->data;
  }

  // Get an array of languages from the server.
  $qtr_server = variable_get('qtrClient_server', 'http://dev.qtranslator.org');
  $output = drupal_http_request("$qtr_server/languages");
  if (isset($output->data)) {
    $languages = json_decode($output->data, TRUE);
  }
  else {
    $languages = array(
      'fr' => array(
        'code' => 'fr',
        'name' => 'French',
        'direction' => LANGUAGE_LTR,
        'plurals' => 2,
      ));
    return $languages;
  }

  // Cache until a general cache wipe.
  cache_set('languages', $languages, 'cache_qtrClient', CACHE_TEMPORARY);

  return $languages;
}
