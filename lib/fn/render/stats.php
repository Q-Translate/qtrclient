<?php
/**
 * @file
 * Function: render_stats()
 */

namespace QTranslate\Client;
use \qcl;

/**
 * Build the content of stats.
 */
function render_stats($lng = 'en') {
  // Get the stats.
  try {
    $qtr = wsclient_service_load('public_qtr');
    $stats = $qtr->report_stats($lng);
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
    $stats = array();
  }

  $content['stats'] = array(
    '#items' => array(),
    '#theme' => 'item_list',
  );
  foreach ($stats as $period => $stat) {
    $nr_likes = $stat['nr_likes'];
    $nr_translations = $stat['nr_translations'];

    $url_likes = url('qtr/search', array(
                   'query' => array(
                     'lng' => $lng,
                     'limit' => '10',
                     'date_filter' => 'likes',
                     'from_date' => $stat['from_date'],
                   )));
    $url_translations = url('qtr/search', array(
                          'query' => array(
                            'lng' => $lng,
                            'limit' => '10',
                            'date_filter' => 'translations',
                            'from_date' => $stat['from_date'],
                          )));

    if ($lng == 'all') {
      $list_item = t("Last !period:", ['!period' => $period]) . "<br/>
        + " . format_plural($nr_likes, '1 like', '@count likes') . "<br/>
        + " . format_plural($nr_translations, '1 translation', '@count translations');
    }
    else {
      $list_item = t("Last !period:", ['!period' => $period])
        . "<br/>
        + <a href='$url_likes' target='_blank'>"
        . format_plural($nr_likes, '1 like', '@count likes')
        . "</a><br/>
        + <a href='$url_translations' target='_blank'>"
        . format_plural($nr_translations, '1 translation', '@count translations')
        . "</a>";
    }
    $content['stats']['#items'][] = $list_item;
  }
  /*
    t('Last week:');
    t('Last month:');
    t('Last year:');
  */

  return $content;
}
