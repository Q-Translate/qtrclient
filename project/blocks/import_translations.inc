<?php
/**
 * @file
 * Import translations.
 */

/**
 * Called from hook_block_view for import_translations.
 */
function qtrProject_import_translations_view() {
  // Get the arguments from the path.
  list($origin, $project, $lng) = qcl::get_project_from_path();

  $block['subject'] = t('Import Translations');
  $block['content'] = drupal_get_form('qtrProject_import_translations_form', $origin, $project, $lng);
  return $block;
}

/**
 * Form callback for: qtrProject_import_translations_form
 */
function qtrProject_import_translations_form($form, &$form_state, $origin, $project, $lng) {
  $description = t('
<p>Importing PO files is for bulk translation and voting. For any translation in
the PO files, it will be added as a suggestion if such a translation does not
exist, or it will just be voted if such a translation already exists. In case
that the translation already exists but its author is not known, then you (the
user who makes the import) will be recorded as the author of the
translation.</p>

<p>This functionality can be useful if you prefer to work off-line with PO
files. You can export the PO files of a project, work on them with desktop
tools (like Lokalize) to translate or correct exported translations, and then
import back to Q-Translate the translated/corrected PO files.</p>
');

  $form = [
    'import_translations' => [
      '#type' => 'fieldset',
      '#title' => t('Import translations from PO files'),
      '#description' => $description,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,

      'file' => [
        '#type' => 'file',
        '#title' => t('PO File(s)'),
        '#description' => t('Upload a PO file. If you have many files, upload them as an archive (tar, tgz, bz2, 7z, zip).'),
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
 * Implements hook form_validate() for qtrProject_import_translations_form.
 */
function qtrProject_import_translations_form_validate($form, &$form_state) {
  qcl::uploadfile_validate();
}


/**
 * Implement hook form_submit() for the form qtrProject_import_translations_form
 */
function qtrProject_import_translations_form_submit($form, &$form_state) {
  if (!oauth2_user_is_authenticated()) {
    oauth2_user_authenticate($form_state, $redirection = ($form===NULL));
    return;
  }

  $messages = qcl::uploadfile_to_server('translations/import', [
                'lng' => qcl::get_translation_lng(),
              ]);
  qcl::display_messages($messages);
}
