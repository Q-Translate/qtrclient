<?php
/**
 * @file
 * Functions specific to the menu path: translations/random
 */

/**
 * Menu callback: translations/random
 *
 * Like the best translation for a verse, or suggest a better translation.
 *
 * If the request header 'Accept: application/json' is set, then
 * the output will be in JSON, otherwise it will be an HTML page.
 */
function qtrTranslations_random($lng = NULL, $vid = NULL) {

  if ($vid == NULL)  $vid = 'random';
  if ($lng == NULL or $lng == 'all')  $lng = qcl::get_translation_lng();

  if (in_array($vid, ['random', 'translated', 'untranslated'])) {
    $qtr = oauth2_user_is_authenticated() ?
      wsclient_service_load('qtr') :
      wsclient_service_load('public_qtr');
    $result = $qtr->get_random_vid([
                'target' => $vid,
                'lng' => $lng,
              ]);
    $vid = $result['vid'];
    if (!$vid) {
      drupal_set_message(t('No verse found!'), 'warning');
      drupal_goto('/');
    }
    else {
      drupal_goto("translations/$lng/$vid");
    }
    return;
  }

  // last_vid is used in search, to get similar results.
  $_SESSION['qtrClient']['last_vid'] = $vid;

  // Get the verse and its translations.
  $qtr = wsclient_service_load('public_qtr');
  $result = $qtr->get_translations($vid, $lng);

  // If the request has the header 'Accept: application/json'
  // or '?format=json' return the output in JSON format and stop.
  qcl::return_json($result);

  // Go to home if there is no verse.
  $verse = $result['verse'];
  if (empty($verse)) {
    drupal_set_message(t('No verse found!'), 'warning');
    drupal_goto('/');
    return;
  }

  // Output the form of the verses and translations.
  $output = drupal_get_form('qtrTranslations_random_form', $vid, $verse, $lng);

  return $output;
}

/**
 * Form callback for: qtrTranslations_random_form
 */
function qtrTranslations_random_form($form, &$form_state, $vid, $verse, $lng) {
  $form = qcl::translateform_build([$vid => $verse], $lng);
  $form['buttons'] += _qtrTranslations_random_form_buttons($lng);
  return $form;
}

/**
 * Get the buttons of the form.
 */
function _qtrTranslations_random_form_buttons($lng) {
  $params = array(
    'attributes' => array(
      'class' => array('btn', 'btn-default', 'form-submit'),
      'style' => 'margin-right:3px',
    ),
  );
  $buttons['random'] = array(
    '#markup' => l(t('Random'), "translations/$lng/",
               $params),
  );
  $buttons['translated'] = array(
    '#markup' => l(t('Translated'), "translations/$lng/translated",
               $params),
  );
  $buttons['untranslated'] = array(
    '#markup' => l(t('Untranslated'), "translations/$lng/untranslated",
               $params),
  );

  return $buttons;
}

/**
 * Submit callback for the form: qtrTranslations_random_form
 */
function qtrTranslations_random_form_submit($form, &$form_state) {
  qcl::translateform_submit($form, $form_state);
}
