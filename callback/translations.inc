<?php
/**
 * @file
 * Functions specific to the menu path: qtr
 */

/**
 * Menu callback: qtr/lng/chapter/verse
 *
 * Get translations of a verse.
 * If the request header 'Accept: application/json' is set, then
 * the output will be in JSON, otherwise it will be an HTML page.
 *
 * Example:
 *     curl -k -H 'Accept: application/json' \
 *          https://qtranslate.net/qtr/en/2/3 \
 *          | python -m json.tool
 *
 */
function qtrClient_translations($lng = NULL, $chapter = NULL, $verse = NULL) {
  // If there is no $chapter/$verse, display the home/dashboard/main page.
  if ($chapter === NULL) {
    include drupal_get_path('module', 'qtrClient') . '/callback/home.inc';
    return qtrClient_home();
  }

  $output[] = qcl::get_block('qtrClient', 'navigate');
  $output[] = qcl::get_block('qtrClient', 'select');

  // Get the verse and its translations.
  $qtr = wsclient_service_load('public_qtr');
  try {
    //$result = $qtr->get_translations("$chapter,$verse,$lng");
    //$result = $qtr->get(compact('lng', 'chapter', 'verse'));
    $result = $qtr->get(array(
                'lng' => $lng,
                'chapter' => $chapter,
                'verse' => $verse,
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
  $output[] = drupal_get_form('qtrClient_form', $verse, $lng);

  return $output;
}

/**
 * Form callback for: qtrClient_form
 */
function qtrClient_form($form, &$form_state, $verse, $lng) {
  if (!isset($verse['vid'])) return array();
  $form = qcl::translateform_build([$verse['vid'] => $verse], $lng);
  $form['buttons'] += _qtrClient_form_buttons($lng);
  return $form;
}

/**
 * Get the buttons of the form.
 */
function _qtrClient_form_buttons($lng) {
  $buttons = array();
  /*
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
  */

  return $buttons;
}

/**
 * Submit callback for the form: qtrClient_form
 */
function qtrClient_form_submit($form, &$form_state) {
  qcl::translateform_submit($form, $form_state);
}
