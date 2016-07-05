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
  $ch_options[''] = t('- Chapter -');
  foreach ($chapters as $ch) {
    $ch_options[$ch['cid']] =  $ch['cid'] . ' : ' . $ch['tname'];
  }
  // Get a list of languages.
  $languages = qcl::get_languages();
  $lang_options[''] = t('- Language -');
  foreach ($languages as $code => $lang) {
    $lang_options[$code] = $lang['name'];
  }

  $form = array(
    'select_verse' => array(
      '#type' => 'fieldset',
      '#title' => t('Select'),

      'chapter' => [
        '#type' => 'select',
        '#title' => t('Chapter'),
        '#options' => $ch_options,
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
        '#options' => $lang_options,
        '#default_value' => $lng,
      ],
    ),

    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Go'),
    ),
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

  if ($chapter < 1 or $chapter > 114) {
    form_set_error('chapter', t('Chapter must be between 1 and 114.'));
  }
  $chapters = qcl::get_chapters();
  if ($verse < 1 or $verse > $chapters[$chapter - 1]['verses']) {
    form_set_error('verse', t('The verse number :chapter,:verse is not correct.',
        [':chapter' => $chapter, ':verse' => $verse]));
  }
  $languages = qcl::get_languages();
  if (!isset($languages[$lng])) {
    form_set_error('lng', t('Language :lng is not supported.', [':lng' => $lng]));
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
