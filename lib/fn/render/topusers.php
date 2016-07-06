<?php
/**
 * @file
 * Function: render_topusers()
 */

namespace QTranslate\Client;
use \qcl;

/**
 * Build the content of top contributors.
 */
function render_topusers($period = 'week', $size = '5', $lng = 'en') {
  // Get the list of top contributers.
  try {
    $qtr = wsclient_service_load('public_qtr');
    $topusers = $qtr->report_topusers($period, $size, $lng);
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
    $topusers = array();
  }

  $from_date = date('Y-m-d', strtotime("-1 $period"));
  $content = array(
    'first_para' => array(
      '#type' => 'markup',
      '#markup' => t("<p>Top !nr translators since !date:</p>",
                 array(
                   '!nr' => $size,
                   '!date' => $from_date,
                 )),
    ),
    'second_para' => array(
      '#theme' => 'item_list',
      '#items' => array(),
    ),
  );

  $qtr_server = variable_get('qtrClient_server');
  foreach ($topusers as $user) {
    $uid = $user['uid'];
    $name = $user['name'];
    $mail = $user['mail'];
    $score = $user['score'];
    $nr_translations = $user['translations'];
    $nr_likes = $user['likes'];
    $url_user = $qtr_server . '/user/' . $user['uid'];

    $url_translations = url('translations/search', array(
                          'query' => array(
                            'lng' => $lng,
                            'translated_by' => $user['name'],
                            'from_date' => $from_date,
                          )));
    $url_likes = url('translations/search', array(
                   'query' => array(
                     'lng' => $lng,
                     'liked_by' => $user['name'],
                     'date_filter' => 'likes',
                     'from_date' => $from_date,
                   )));

    if ($lng == 'all') {
      $list_item = "
      <strong><a href='$url_user' target='_blank'>$name</a></strong><br/>
        + " . format_plural($nr_likes, '1 like', '@count likes') . " <br/>
        + " . format_plural($nr_translations, '1 translation', '@count translations') . " ";
    }
    else {
      $list_item = "
      <strong><a href='$url_user' target='_blank'>$name</a></strong><br/>
        + <a href='$url_likes' target='_blank'>"
        . format_plural($nr_likes, '1 like', '@count likes')
        . " </a><br/>
        + <a href='$url_translations' target='_blank'>"
        . format_plural($nr_translations, '1 translation', '@count translations')
        . " </a> ";
    }

    $content['second_para']['#items'][] = $list_item;
  }

  return $content;
}
