<?php
/**
 * @file
 * Function: translateform_buttons()
 */

namespace QTranslate\Client;
use \qcl;

/**
 * Get the buttons of the form as a render array.
 */
function translateform_buttons($lng, $verse = NULL) {
  // The save button will appear only when the user has
  // permissions to submit likes and translations.
  $translation_lng = variable_get('qtrClient_translation_lng', 'all');
  $enable_save = ($translation_lng == $lng or $translation_lng == 'all');
  $buttons['save'] = [
    '#type' => 'submit',
    '#value' => t('Save'),
    '#access' => $enable_save,
    '#attributes' => ['onclick' => 'this.form.target="_self"'],
  ];
  // When the user is not authenticated, clicking Save will redirect
  // to login. When we are inside an iFrame, it is better to do the
  // login on a popup window (or a new tab).
  if (!oauth2_user_is_authenticated() and inside_iframe()) {
    $buttons['save']['#attributes']['onclick'] = 'this.form.target="_blank"';
  }

  if ($verse !== NULL) {
    $buttons += qcl::translateform_meta($lng, $verse);
  }
  else {
    $buttons['save']['#attributes']['style'] = 'margin-left: 50%;';
  }

  return $buttons;
}
