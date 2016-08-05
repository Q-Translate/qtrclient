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

  // Get the words to searche for.
  if (trim($form_values['words']) != '') {
    $query['words'] = $form_values['words'];
  }

  // Get the RTL button.
  if ($form_values['rtl'] == 1) {
    $query['rtl'] = '1';
  }

  // Get the advanced search button.
  if ($form_values['adv'] == 1) {
    $query['adv'] = '1';
  }

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

  // Get limit.
  list($limit_options, $default_limit) = qcl::filter_get_options('limit');
  if (in_array($form_values['limit'], $limit_options) && $form_values['limit'] != $default_limit) {
    $query['limit'] = $form_values['limit'];
  }

  // Get the page.
  $page = (int)$form_values['page'] - 1;
  if ($page > 0) {
    $query['page'] = $page;
  }

  // Skip the advanced search fields for the anonymous users.
  if (!oauth2_user_is_authenticated())  return $query;

  // Get search type.
  list($type_options, $default_type) = qcl::filter_get_options('type');
  if (in_array($form_values['type'], $type_options) && $form_values['type'] != $default_type) {
    $query['type'] = $form_values['type'];
  }

  // Get what to search.
  list($what_options, $default_what) = qcl::filter_get_options('what');
  if (in_array($form_values['what'], $what_options) && $form_values['what'] != $default_what) {
    $query['what'] = $form_values['what'];
  }

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

  return $query;
}
