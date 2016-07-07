<?php
/**
 * @file
 * Functions for the block search.
 */

/**
 * Called from hook_block_view for search.
 */
function qtrTranslations_search_view() {
  // Get the arguments from the path.
  list($lng, $_, $_) = qcl::get_args_from_path();

  //$block['subject'] = t('Search');
  $block['content'] = drupal_get_form('qtrTranslations_search_form', $lng);
  return $block;
}

/**
 * Form callback for: qtrTranslations_search_form
 */
function qtrTranslations_search_form($form, &$form_state, $lng) {
  // Get a list of languages.
  $languages = qcl::get_languages();
  $language_list[''] = t('- Language -');
  foreach ($languages as $code => $lang) {
    $language_list[$code] = $lang['name'];
  }
  // Get a list of chapters.
  $chapters = qcl::get_chapters();
  $chapter_list[''] = t('- Chapter -');
  for ($c = 1; $c <=114; $c++) {
    $chapter_list["$c"] = $c . ' : ' . $chapters[$c]['tname'];
  }

  $form['search_translations'] = array(

    '#type' => 'fieldset',
    '#title' => t('Search'),

    'lng' => [
      '#type' => 'select',
      '#title' => t('Language'),
      '#options' => $language_list,
      '#default_value' => $lng,
    ],

    'chapter' => [
      '#type' => 'select',
      '#title' => t('Chapter'),
      '#options' => $chapter_list,
    ],

    'words' => [
      '#type' => 'textfield',
      '#title' => t('Words'),
    ],

    'submit' => [
      '#type' => 'submit',
      '#value' => t('Search'),
    ],
  );

  return $form;
}

/**
 * Validate callback for the form: qtrTranslations_search_form
 */
function qtrTranslations_search_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $chapter = $values['chapter'];
  $words = $values['words'];
  $lng = $values['lng'];

  if ($words == '') {
    form_set_error('words', t('Search box is empty.'));
    return;
  }

  if ($lng == '')  $lng = variable_get('qtrClient_translation_lng', 'en');
  if ($lng == 'all')  $lng = 'en';
  $form_state['values']['lng'] = $lng;

  $languages = qcl::get_languages();
  if (!isset($languages[$lng])) {
    form_set_error('lng', t('Language :lng is not supported.', [':lng' => $lng]));
    return;
  }
}

/**
 * Submit callback for the form: qtrTranslations_search_form
 */
function qtrTranslations_search_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $chapter = $values['chapter'];
  $words = $values['words'];
  $lng = $values['lng'];

  $query['words'] = $words;
  if ($lng) $query['lng'] = $lng;
  if ($chapter) $query['chapter'] = $chapter;
  drupal_goto('/qtr/search', ['query' => $query]);
}
