<?php
/**
 * @file
 * Function: filter_get_params()
 */

namespace QTranslate\Client;
use \qcl;

/**
 * Get the filter parameters from the GET request.
 *
 * The GET request is like: qtr/search?lng=en&words=unseen&limit=10
 */
function filter_get_params() {
  // Get a copy of the request parameters.
  $request = $_GET;

  // Get and check the language.
  $lng = isset($request['lng']) ? trim($request['lng']) : '';
  $languages = qcl::get_languages();
  $lng_codes = array_keys($languages);
  $params['lng'] = in_array($lng, $lng_codes) ? $lng : 'en';

  // Number of results to be displayed.
  if (isset($request['limit'])) {
    $limit = (int) trim($request['limit']);
    list($limit_options, $default_limit) = qcl::filter_get_options('limit');
    $params['limit'] = in_array($limit, $limit_options) ? $limit : $default_limit;
  }
  // Page of results to be displayed.
  if (isset($request['page'])) {
    $params['page'] = (int) trim($request['page']);
  }

  // Search can be done either by similarity of words (natural search),
  // or by matching words according to a certain logic (boolean search).
  // Search can be performed either on verses or on the translations.
  if (isset($request['mode'])) {
    $mode = $request['mode'];
    list($search_mode_options, $default_search_mode) = qcl::filter_get_options('mode');
    $params['mode'] = in_array($mode, $search_mode_options) ? $mode : $default_search_mode;
  }
  if (isset($request['words'])) {
    $params['words'] = $request['words'];
  }

  // Searching can be limited to a chapter.
  if (isset($request['chapter'])) {
    $params['chapter'] = trim($request['chapter']);
  }

  // Limit search only to the verses touched (translated or liked)
  // by the current user.
  if (isset($request['only_mine']) && (int) $request['only_mine']) {
    $params['only_mine'] = 1;
  }

  // Limit search by the editing users (used by admins).
  if (isset($request['translated_by'])) {
    $params['translated_by'] = trim($request['translated_by']);
  }
  if (isset($request['liked_by'])) {
    $params['liked_by'] = trim($request['liked_by']);
  }

  // Limit by date of translation or voting (used by admins).
  if (isset($request['date_filter'])) {
    $date_filter = trim($request['date_filter']);
    list($date_filter_options, $default_date_filter) = qcl::filter_get_options('date_filter');
    $params['date_filter'] = in_array($date_filter, $date_filter_options) ? $date_filter : $default_date_filter;
  }

  // Get from_date.
  if (isset($request['from_date'])) {
    $from_date = trim($request['from_date']);
    if ($from_date != '') {
      $from_date = date('Y-m-d H:i:s', strtotime($from_date));
      $from_date = str_replace(' 00:00:00', '', $from_date);
    }
    $params['from_date'] = $from_date;
  }

  // Get to_date.
  if (isset($request['to_date'])) {
    $to_date = trim($request['to_date']);
    if ($to_date != '') {
      $to_date = date('Y-m-d H:i:s', strtotime($to_date));
      $to_date = str_replace(' 00:00:00', '', $to_date);
    }
    $params['to_date'] = $to_date;
  }

  return $params;
}
