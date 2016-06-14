<?php
/**
 * @file
 * Return a random string in a suitable form for the twitter.
 */

/**
 * Callback for 'qtr/tweet'.
 */
function qtrClient_tweet($origin = NULL, $project = NULL, $lng = NULL) {
  // Get the translation language.
  if ($lng === NULL)  $lng = qcl::get_translation_lng();

  // Get a tweet from the Q-Translate server.
  $qtr_server = variable_get('qtrClient_server', 'http://dev.qtranslate.org');
  $url = $qtr_server . "/tweet/$lng/$origin/$project";
  $output = drupal_http_request($url, array(
              'headers' => array(
                'Accept' => 'application/json',
              )));
  $result = json_decode($output->data, TRUE);
  $sguid = $result['sguid'];
  $twitter = $result['twitter'];

  // Append hash tags.
  $twitter .= " #l10n_$lng";
  if (!empty($origin)) {
    $twitter .= " #$origin";
  }
  if (!empty($project)) {
    $twitter .= " #$project";
  }

  // Append the url.
  if ((empty($origin) and empty($project)) or ($origin == 'vocabulary')) {
    if (empty($project)) {
      $project = "ICT_$lng";
    }
    list($term, $x) = explode(' <==> ', $twitter, 2);
    if ($project == 'ICT_sq') {
      $url = "http://fjalori.fs.al/#$term";
    }
    else {
      $url = url("vocabulary/$project/$term", array('absolute' => TRUE));
    }
  }
  else {
    $url = url("translations/$lng/$sguid", array('absolute' => TRUE));
  }
  $twitter .= ' ' . $url;

  print $twitter;
  exit(0);
}
