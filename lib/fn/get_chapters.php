<?php
/**
 * @file
 * Function: get_chapters()
 */

namespace QTranslate\Client;

/**
 * Return an array of chapters and their details.
 */
function get_chapters() {
  $cache = cache_get('chapters', 'cache_qtrClient');
  // Return cache if possible.
  if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
    return $cache->data;
  }

  // Get an array of chapters from the server.
  $qtr_server = variable_get('qtrClient_server', 'https://dev.qtranslate.org');
  $output = drupal_http_request("$qtr_server/chapters");
  if (isset($output->data)) {
    $chapters = json_decode($output->data, TRUE);
  }
  else {
    return array();
  }

  // Cache until a general cache wipe.
  cache_set('chapters', $chapters, 'cache_qtrClient', CACHE_TEMPORARY);

  return $chapters;
}
