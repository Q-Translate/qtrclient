<?php
/**
 * @file
 * Menu callback for qtr/search
 */

/**
 * Menu callback: qtr/search.
 *
 * Search verses and translations using various filters.
 *
 * If the request header 'Accept: application/json' is set, then
 * the output will be in JSON, otherwise it will be an HTML page.
 * For example:
 *   curl -k -i -H 'Accept: application/json' \
 *        'https://qtranslate.org/qtr/search?words=unseen&chapter=2'
 */
function qtrClient_search() {
  // Get the filter parameters from the GET request.
  $params = qcl::filter_get_params();

  // Get the verses that match the search params.
  $qtr = oauth2_user_is_authenticated() ?
    wsclient_service_load('qtr') :
    wsclient_service_load('public_qtr');
  $result = $qtr->search($params);

  // If the request has the header 'Accept: application/json'
  // or '?format=json' return the output in JSON format and stop.
  qcl::return_json($result['verses']);

  // Set the title.
  drupal_set_title(t('Search Translations'));

  // Add the filter form.
  $output[] = drupal_get_form('qtrClient_filter_form', $result['filter']);

  // Output the verses.
  $verses = $result['verses'];
  $pager = $result['pager'];
  if (empty($verses)) {
    drupal_set_message(t('No translations found. Try adjusting the filter options.'));
  }
  else {
    pager_default_initialize($pager['number_of_items'], $pager['items_per_page']);
    $lng = (isset($params['lng']) ? $params['lng'] : qcl::get_translation_lng());
    $output[] = drupal_get_form('qtrClient_result_form', $verses, $lng);
  }

  return $output;
}

/**
 * Form callback for: qtrClient_filter_form
 */
function qtrClient_filter_form($form, &$form_state, $filter) {
  // If the form has no values yet (either it is the first time that
  // the form is opened, or we have a get request), we set the values
  // of the form according to the $filter values.
  if (empty($form_state['values'])) {
    $form_state['values'] = $filter;
  }
  $form = qcl::filter_form($form_state['values']);
  return $form;
}

/**
 * Submit callback for the form: qtrClient_filter_form
 */
function qtrClient_filter_form_submit($form, &$form_state) {
  module_load_include('php', 'qtrClient', 'lib/fn/filter/submit');
  \QTranslate\Client\filter_submit($form, $form_state);
}

/**
 * Form callback for: qtrClient_result_form
 */
function qtrClient_result_form($form, &$form_state, $verses, $lng) {
  $form = qcl::translateform_build($verses, $lng);
  return $form;
}

/**
 * Submit callback for the form: qtrClient_result_form
 */
function qtrClient_result_form_submit($form, &$form_state) {
  qcl::translateform_submit($form, $form_state);
}
