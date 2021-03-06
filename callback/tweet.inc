<?php
/**
 * @file
 * Return a random translation in a suitable form for the twitter.
 *
 * Example:
 *   curl -q -k https://qtranslate.net/qtr/tweet/en
 */

/**
 * Callback for 'qtr/tweet'.
 */
function qtrClient_tweet($lng = NULL) {
  // Get the translation language.
  if ($lng === NULL)  $lng = qcl::get_translation_lng();

  // Get a tweet from the Q-Translate server.
  $qtr_server = variable_get('qtrClient_server', 'https://qtranslate.org');
  $url = $qtr_server . "/tweet/$lng";
  $output = qcl::http_request($url, array(
              'headers' => array(
                'Accept' => 'application/json',
              )));
  $result = json_decode($output->data, TRUE);
  $cid = $result['chapter'];
  $nr = $result['verse'];
  $tweet = $result['tweet'];

  // Append hash tags, url, etc.
  $tweet = "[Quran $cid:$nr] $tweet #quran_$lng";
  if (isset($_GET['url'])) {
    $url = $_GET['url'];
  }
  else {
    $url = url("qtr/$lng/$cid/$nr", array('absolute' => TRUE));
  }
  $tweet .= ' ' . $url;

  print $tweet;
  exit(0);
}
