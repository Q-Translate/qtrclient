<?php
/**
 * @file
 * Function: filter_get_query()
 */

namespace QTranslate\Client;
use \qcl;

/**
 * Converts the array of form values into a suitable query (for redirect).
 *
 * All non-string values are converted to string representations.
 * If a form value is equal to the default value, it is removed (unset)
 * from the array.
 */
function filter_get_query($form_values) {
  $query = array();

  // Get lng.
  $languages = qcl::get_languages();
  $lng_codes = array_keys($languages);
  $lng = trim($form_values['lng']);
  if (in_array($lng, $lng_codes)) {
    $query['lng'] = $lng;
  }

  // Get chapter.
  if (trim($form_values['chapter']) != '') {
    $query['chapter'] = $form_values['chapter'];
  }

  if (oauth2_user_is_authenticated()) {
    // Get only_mine.
    if ($form_values['only_mine'] == 1) {
      $query['only_mine'] = '1';
    }
    else {
      // Get translated_by, liked_by.
      if (trim($form_values['translated_by']) != '') {
        $query['translated_by'] = $form_values['translated_by'];
      }
      if (trim($form_values['liked_by']) != '') {
        $query['liked_by'] = $form_values['liked_by'];
      }
    }

    // Get date_filter.
    list($date_filter_options, $default_date_filter) = qcl::filter_get_options('date_filter');
    if (in_array($form_values['date_filter'], $date_filter_options) && $form_values['date_filter'] != $default_date_filter) {
      $query['date_filter'] = $form_values['date_filter'];
    }

    // Get from_date.
    if (trim($form_values['from_date']) != '') {
      $query['from_date'] = $form_values['from_date'];
    }

    // Get to_date.
    if (trim($form_values['to_date']) != '') {
      $query['to_date'] = $form_values['to_date'];
    }
  }

  // Get limit.
  list($limit_options, $default_limit) = qcl::filter_get_options('limit');
  if (in_array($form_values['limit'], $limit_options) && $form_values['limit'] != $default_limit) {
    $query['limit'] = $form_values['limit'];
  }

  // Get search mode and words.
  list($search_mode_options, $default_search_mode) = qcl::filter_get_options('mode');
  if (in_array($form_values['mode'], $search_mode_options) && $form_values['mode'] != $default_search_mode) {
    $query['mode'] = $form_values['mode'];
  }
  if (trim($form_values['words']) != '') {
    $query['words'] = $form_values['words'];
  }

  // Get the page.
  $page = (int)$form_values['page'] - 1;
  if ($page > 0) {
    $query['page'] = $page;
  }

  return $query;
}
