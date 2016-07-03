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
function translateform_buttons($lng, $vid = NULL) {
  if ($vid and inside_iframe()) {
    $buttons['details'] = [
      '#markup' => l('#', "translations/$lng/$vid", [
                   'attributes' => [
                     'class' => ['btn', 'btn-default'],
                     'style' => 'font-weight:bold; margin-right:3px;',
                     'target' => '_blank',
                   ]]),
    ];
  }

  // The save button will appear only when the user has
  // permissions to submit likes and suggestions.
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

  return $buttons;
}
