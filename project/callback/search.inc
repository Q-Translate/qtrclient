<?php
/**
 * @file
 * Callback for menu item: 'qtr/project/%/%/%/search'
 */

/**
 * Menu callback for 'qtr/project/%/%/%/search'
 *
 * Search strings and translations using various filters.
 *
 * If the request header 'Accept: application/json' is set, then
 * the output will be in JSON, otherwise it will be an HTML page.
 */
function qtrProject_search($origin, $project, $lng) {
  // Get the filter parameters from the GET request.
  $params = qcl::filter_get_params();
  $params['origin'] = $origin;
  $params['project'] = $project;
  $params['lng'] = $lng;

  // Get the strings that match the search params.
  $qtr = oauth2_user_is_authenticated() ?
    wsclient_service_load('qtr') :
    wsclient_service_load('public_qtr');
  $result = $qtr->search($params);

  // If the request has the header 'Accept: application/json'
  // or '?format=json' return the output in JSON format and stop.
  qcl::return_json($result['strings']);

  // Set the title.
  drupal_set_title(t('Search'));

  // Add the filter form.
  $output[] = drupal_get_form('qtrProject_filter_form', $result['filter']);

  // Output the strings.
  $strings = $result['strings'];
  if (empty($strings)) {
    drupal_set_message(t('No strings found. Try adjusting the filter options.'));
  }
  else {
    $pager = $result['pager'];
    pager_default_initialize($pager['number_of_items'], $pager['items_per_page']);
    $output[] = drupal_get_form('qtrProject_search_form', $strings, $lng);

  }

  return $output;
}

/**
 * Form callback for: qtrProject_filter_form
 */
function qtrProject_filter_form($form, &$form_state, $filter) {
  // If the form has no values yet (either it is the first time that
  // the form is opened, or we have a get request), we set the values
  // of the form according to the $filter values.
  if (empty($form_state['values'])) {
    $form_state['values'] = $filter;
  }
  $form = qcl::filter_form($form_state['values']);
  unset($form['advanced']['lng']);
  unset($form['advanced']['projects']);
  return $form;
}

/**
 * Submit callback for the form: qtrProject_filter_form
 */
function qtrProject_filter_form_submit($form, &$form_state) {
  module_load_include('php', 'qtrClient', 'lib/fn/filter/submit');
  \BTranslator\Client\filter_submit($form, $form_state);
}

/**
 * Form callback for: qtrProject_search_form
 */
function qtrProject_search_form($form, &$form_state, $strings, $lng) {
  $form = qcl::translateform_build($strings, $lng);
  return $form;
}

/**
 * Submit callback for the form: qtrProject_search_form
 */
function qtrProject_search_form_submit($form, &$form_state) {
  qcl::translateform_submit($form, $form_state);
}
