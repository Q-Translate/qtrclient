<?php
/**
 * @file
 * Function: http_request()
 */

namespace QTranslate\Client;

/**
 * Call drupal_http_request() with proper options for skipping ssl verification.
 */
function http_request($url, $options = []) {
  $options['context'] = stream_context_create([
                          'ssl' => [
                            'verify_peer' => FALSE,
                            'verify_peer_name' => FALSE,
                          ]]);
  return drupal_http_request($url, $options);
}
