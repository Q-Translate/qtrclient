<?php
/**
 * @file
 * Import a project.
 */

/**
 * Called from hook_block_view for import.
 */
function qtrProject_import_view() {
  // Get the arguments from the path.
  list($origin, $project, $lng) = qcl::get_project_from_path();

  $block['subject'] = t('Import Project');
  $block['content'] = drupal_get_form('qtrProject_import_form', $origin, $project, $lng);
  return $block;
}

/**
 * Form callback for: qtrProject_import_form
 */
function qtrProject_import_form($form, &$form_state, $origin, $project, $lng) {
  $description = t('

<p>Create a project or update an existing one by importing the
<strong>POT</strong> and <strong>PO</strong> files of the project.
<ul>
  <li>Put all the files inside an archive of type: <strong>tgz, bz2, 7z, zip</strong></li>
  <li>The POT files should be placed in the subfolder <strong>pot/</strong>,
      directly in the root folder of the archive.</li>
  <li>The PO files of a language should be placed in the subfolder named after the code
      of that language. Supported language codes are these: <strong>!lng_codes</strong></li>
  <li> The path and name of each PO file should be the same as the corresponding POT file,
       except for the extension (.po instead of .pot).</li>
</ul>
</p>',
                 ['!lng_codes' => implode(', ', array_keys(qcl::get_languages()))]
  );

  // Suggest the username as the default value for the origin.
  $qtr_user = oauth2_user_get();
  $default_origin = str_replace(' ', '', $qtr_user['name']);
  if (!$default_origin) $default_origin = 'test';

  // Build the form.
  $form = [
    'import' => [
      '#type' => 'fieldset',
      '#title' => t('Import the POT and PO files of a project'),
      '#description' => $description,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,

      'origin' => [
        '#type' => 'textfield',
        '#title' => t('Origin'),
        '#default_value' => $default_origin,
        '#required' => TRUE,
      ],

      'project' => [
        '#type' => 'textfield',
        '#title' => t('Project'),
        '#required' => TRUE,
      ],

      'file' => [
        '#type' => 'file',
        '#title' => t('Archive of POT/PO files (tgz, bz2, 7z, zip)'),
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
 * Implements hook form_validate() for qtrProject_import_form.
 */
function qtrProject_import_form_validate($form, &$form_state) {
  $extensions = 'tgz bz2 7z zip';
  qcl::uploadfile_validate($extensions);
}


/**
 * Implement hook form_submit() for the form qtrProject_import_form
 */
function qtrProject_import_form_submit($form, &$form_state) {
  if (!oauth2_user_is_authenticated()) {
    oauth2_user_authenticate($form_state, $redirection = ($form===NULL));
    return;
  }

  $messages = qcl::uploadfile_to_server('project/import', [
                'origin' => $form_state['values']['origin'],
                'project' => $form_state['values']['project'],
              ]);
  qcl::display_messages($messages);
}
