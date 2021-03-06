<?php
/**
 * @file
 * Function: filter_get_options()
 */

namespace QTranslate\Client;

/**
 * Return an array of options for the given field and the default value.
 *
 * If being associative ($assoc) is not required then only the keys are
 * returned.
 */
function filter_get_options($field, $assoc = FALSE) {

  switch ($field) {

    case 'limit':
      // Number of results to be displayed per page.
      $options = array(
        '1'  => '1',
        '5'  => '5',
        '10' => '10',
        '20' => '20',
        '30' => '30',
        '50' => '50',
        '100' => '100',
      );
      $default = 5;
      break;

    case 'type':
      // The type of search.
      $options = array(
        'similar' => t('Similar'),
        'logical' => t('Logical'),
      );
      $default = 'similar';
      break;

    case 'what':
      // What to search.
      $options = array(
        'translations' => t('Translations'),
        'verses' => t('Verses'),
      );
      $default = 'translations';
      break;

    case 'date_filter':
      // Which date to filter.
      $options = array(
        'translations' => t('Filter Translations By Date'),
        'likes' => t('Filter Likes By Date'),
      );
      $default = 'translations';
      break;
  }
  if (!$assoc) {
    $options = array_keys($options);
  }

  return array($options, $default);
}
