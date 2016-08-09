<?php
/**
 * @file
 * Function: get_languages()
 */

namespace QTranslate\Client;
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
  $qtr_server = variable_get('qtrClient_server', 'https://qtranslate.org');
  $output = qcl::http_request("$qtr_server/languages");
  if (isset($output->data)) {
    $languages = json_decode($output->data, TRUE);
  }
  else {
    $languages = array(
      'en' => array(
        'code' => 'en',
        'name' => 'English',
        'direction' => LANGUAGE_LTR,
      ));
    return $languages;
  }

  // Cache until a general cache wipe.
  cache_set('languages', $languages, 'cache_qtrClient', CACHE_TEMPORARY);

  return $languages;
}
