<?php
/**
 * @file
 * Administrative pages for the qtrClient module.
 */

/**
 * General configuration settings for the module qtrClient.
 *
 * @return array
 *   An array containing form items to place on the module settings page.
 */
function qtrClient_config() {

  $form['defaults'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Q-Translate Client Default Settings'),
  );

  // Get a list of available languages,
  $languages = qcl::get_languages();
  $lang_options['all'] = t('All languages');
  foreach ($languages as $code => $lang) {
    $lang_options[$code] = $lang['name'];
  }

  // Field qtrClient_translation_lng.
  $form['defaults']['qtrClient_translation_lng'] = array(
    '#type' => 'select',
    '#title' => t('Translation Language'),
    '#description' => t('The translation language of the client. If <em>All languages</em> is selected, the client will not restrict the language of the translations and likes (it will be restricted only by the translation language of the user).'),
    '#options' => $lang_options,
    '#default_value' => variable_get('qtrClient_translation_lng', 'en'),
  );

  // Field qtrClient_server.
  $form['defaults']['qtrClient_server'] = array(
    '#type' => 'textfield',
    '#title' => t('Server URL'),
    '#default_value' => variable_get('qtrClient_server', ''),
    '#description' => t('The URL of the Q-Translate server.'),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'qtrClient_config_get_languages';
  return $form;
}

/**
 * Update the cached value of languages.
 */
function qtrClient_config_get_languages() {
  cache_clear_all('qtrClient_languages', 'cache_qtrClient');
  $languages = qcl::get_languages();
}
