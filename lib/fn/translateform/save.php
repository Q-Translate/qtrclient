<?php
/**
 * @file
 * Function: translateform_save()
 */

namespace QTranslate\Client;
use \qcl;

/**
 * Save the values selected on the form (likes or new translations).
 */
function translateform_save($form_values) {
  // Get the langcode submitted with the form.
  $lng = $form_values['langcode'];

  // Iterate outer structure built in qcl::translateform_build().
  foreach ($form_values['verses'] as $vid => $verse) {

    _submit($vid, $lng, $verse);

    // Iterate the inner structure built in qcl::translateform_verse().
    // Form items have numeric $tguid values and other keys here.
    foreach ($verse as $tguid => $translation) {
      if ((strlen($tguid) == 40) && !empty($translation['declined'])) {
        // Delete translation.
        _add_action('del', array('tguid' => $tguid));
      }
    }
  }

  // Submit the actions.
  global $_qtrclient_actions;
  if (empty($_qtrclient_actions)) {
    return;
  }
  $qtr = wsclient_service_load('qtr');
  $result = $qtr->submit($_qtrclient_actions);

  // Display any messages, warnings and errors.
  qcl::display_messages($result['messages']);
}

/**
 * Add an action to the list of actions.
 */
function _add_action($action, $params) {
  global $_qtrclient_actions;
  $_qtrclient_actions[] = array('action' => $action, 'params' => $params);
}

/**
 * Return true if a new translation has been submitted.
 */
function _not_empty_translation($translation) {
  $translation = str_replace(t('<New translation>'), '', $translation);
  $translation = trim($translation);
  return !empty($translation);
}

/**
 * Form submit for the case of voting mode 'single'.
 */
function _submit($vid, $lng, $verse) {

  if (_not_empty_translation($verse['new']['value'])) {
    // Add a new translation.
    _add_action('add', array(
        'vid' => $vid,
        'lng' => $lng,
        'translation' => $verse['new']['value'],
      ));
    return;
  }

  $tguid = $verse['approved'];
  if (strlen($tguid) == 40) {
    // If this is not an existing like,
    // then add a new like for this translation.
    $previous_likes = $verse[$tguid]['original']['likes'];
    $qtr_user = oauth2_user_get();
    if (!in_array($qtr_user['name'], array_keys($previous_likes))) {
      _add_action('like', array('tguid' => $tguid));
    }
  }
}
