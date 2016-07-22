<?php
/**
 * @file
 * Import a project.
 */

/**
 * Called from hook_block_view for import.
 */
function qtrClient_import_view() {
  // Get the arguments from the path.
  list($lng, $_, $_) = qcl::get_args_from_path();

  //$block['subject'] = t('Import');
  $block['content'] = drupal_get_form('qtrClient_import_form', $lng);
  return $block;
}

/**
 * Form callback for: qtrClient_import_form
 */
function qtrClient_import_form($form, &$form_state, $lng) {
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

  // Build the form.
  $form = [
    'import' => [
      '#type' => 'fieldset',
      '#title' => t('Import'),
      //'#description' =>  t('The translation of each verse must be in a new line.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,

      'lng' => [
        '#type' => 'select',
        '#title' => t('Language'),
        '#options' => $language_list,
        '#default_value' => $lng,
      ],

      'author' => [
        '#type' => 'textfield',
        '#title' => t('Author'),
        '#default_value' => $GLOBALS['user']->name,
        '#disabled' => !qcl::user_access('qtranslate-admin'),
      ],

      'file' => [
        '#type' => 'file',
        '#title' => t('Translations (text file)'),
      ],

      'submit' => [
        '#value' => t('Import'),
        '#type' => 'submit',
      ],
    ],
  ];
  return $form;
}

/**
 * Implements hook form_validate() for qtrClient_import_form.
 */
function qtrClient_import_form_validate($form, &$form_state) {
  // Check that a file is uploaded.
  $file_tmp_name = $_FILES['files']['tmp_name']['file'];
  if ($file_tmp_name == '') {
    form_set_error('file', t("Please select a file."));
    return;
  }

  // Check for any other upload errors.
  $file_error = $_FILES['files']['error']['file'];
  if ($file_error != 0) {
    form_set_error('file',
      t("Error %error happened during file upload.",
        ['%error' => $file_error])
    );
    return;
  }

  // Check the type of the uploaded file.
  $ftype = $_FILES['files']['type']['file'];
  if ($ftype != 'text/plain') {
    form_set_error('file', t('File must be of type: text/plain'));
    return;
  }
}


/**
 * Implement hook form_submit() for the form qtrClient_import_form
 */
function qtrClient_import_form_submit($form, &$form_state) {
  if (!oauth2_user_is_authenticated()) {
    oauth2_user_authenticate($form_state, $redirection = ($form===NULL));
    return;
  }

  // Check permissions.
  if (!qcl::user_access('qtranslate-import') && !qcl::user_access('qtranslate-admin')) {
    drupal_set_message(t('Only translators or admins are allowed to import.'), 'error');
    return;
  }

  $messages = qcl::uploadfile('translations/import', [
                'lng' => $form_state['values']['lng'],
                'author' => $form_state['values']['author'],
              ]);
  qcl::display_messages($messages);
}
