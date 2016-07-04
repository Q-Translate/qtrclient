<?php
/**
 * @file
 * Functions specific to the menu path: translations
 */

/**
 * Menu callback: translations/chapter/verse/lng
 *
 * Get translations of a verse.
 * If the request header 'Accept: application/json' is set, then
 * the output will be in JSON, otherwise it will be an HTML page.
 *
 * Example:
 *     curl -k -H 'Accept: application/json' \
 *          https://qtranslate.net/translations/2/3/en \
 *          | python -m json.tool
 *
 */
function qtrTranslations_get($chapter = NULL, $verse = NULL, $lng = NULL) {
  // Get the verse and its translations.
  $qtr = wsclient_service_load('public_qtr');
  try {
    //$result = $qtr->get_translations("$chapter,$verse,$lng");
    //$result = $qtr->get(compact('chapter', 'verse', 'lng'));
    $result = $qtr->get(array(
                'chapter' => $chapter,
                'verse' => $verse,
                'lng' => $lng,
              ));
  }
  catch (WSClientException $e) {
    drupal_set_message($e->getMessage(), 'error');
  }

  // If the request has the header 'Accept: application/json'
  // or '?format=json' return the output in JSON format and stop.
  qcl::return_json($result);

  // Output the form of the verses and translations.
  $verse = $result['verse'];
  $output = drupal_get_form('qtrTranslations_form', $verse, $lng);

  return $output;
}

/**
 * Form callback for: qtrTranslations_form
 */
function qtrTranslations_form($form, &$form_state, $verse, $lng) {
  if (!isset($verse['vid'])) return array();
  $form = qcl::translateform_build([$verse['vid'] => $verse], $lng);
  $form['buttons'] += _qtrTranslations_form_buttons($lng);
  return $form;
}

/**
 * Get the buttons of the form.
 */
function _qtrTranslations_form_buttons($lng) {
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
 * Submit callback for the form: qtrTranslations_form
 */
function qtrTranslations_form_submit($form, &$form_state) {
  qcl::translateform_submit($form, $form_state);
}
