<?php
/**
 * @file
 * Functions for the block select.
 */

/**
 * Called from hook_block_view for select.
 */
function qtrTranslations_select_view() {
  // Get the arguments from the path.
  list($chapter, $verse, $lng) = qcl::get_args_from_path();

  //$block['subject'] = t('Select');
  $block['content'] = drupal_get_form('qtrTranslations_select_form', $chapter, $verse, $lng);
  return $block;
}

/**
 * Form callback for: qtrTranslations_select_form
 */
function qtrTranslations_select_form($form, &$form_state, $chapter, $verse, $lng) {
  // Get a list of chapters.
  $chapters = qcl::get_chapters();
  $chapter_list[''] = t('- Chapter -');
  for ($c = 1; $c <=114; $c++) {
    $chapter_list["$c"] = $c . ' : ' . $chapters[$c]['tname'];
  }
  // Get a list of languages.
  $languages = qcl::get_languages();
  $language_list[''] = t('- Language -');
  foreach ($languages as $code => $lang) {
    $language_list[$code] = $lang['name'];
  }

  $form['select_translations'] = array(

    '#type' => 'fieldset',
    '#title' => t('Select'),

    'chapter' => [
      '#type' => 'select',
      '#title' => t('Chapter'),
      '#options' => $chapter_list,
      '#default_value' => $chapter,
    ],

    'verse' => [
      '#type' => 'textfield',
      '#title' => t('Verse'),
      '#default_value' => $verse,
      '#size' => 3,
      '#maxlength' => 3,
    ],

    'lng' => [
      '#type' => 'select',
      '#title' => t('Language'),
      '#options' => $language_list,
      '#default_value' => $lng,
    ],

    'submit' => [
      '#type' => 'submit',
      '#value' => t('Go'),
    ],
  );

  return $form;
}

/**
 * Validate callback for the form: qtrTranslations_select_form
 */
function qtrTranslations_select_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $chapter = $values['chapter'];
  $verse = $values['verse'];
  $lng = $values['lng'];

  if ($chapter == '') {
    form_set_error('chapter', t('No chapter selected.'));
    return;
  }

  if ($chapter < 1 or $chapter > 114) {
    form_set_error('chapter', t('Chapter must be between 1 and 114.'));
    return;
  }

  if ($verse == '') {
    $verse = 1;
    $form_state['values']['verse'] = 1;
  }

  $chapters = qcl::get_chapters();
  if ($verse < 1 or $verse > $chapters[$chapter]['verses']) {
    form_set_error('verse', t('The verse number :chapter,:verse is not correct.',
        [':chapter' => $chapter, ':verse' => $verse]));
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
 * Submit callback for the form: qtrTranslations_select_form
 */
function qtrTranslations_select_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $chapter = $values['chapter'];
  $verse = $values['verse'];
  $lng = $values['lng'];
  drupal_goto("/qtr/$chapter/$verse/$lng");
}
