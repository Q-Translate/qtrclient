<?php
/**
 * @file
 * Functions for the block export.
 */

/**
 * Called from hook_block_view for export.
 */
function qtrTranslations_export_view() {
  // Get the arguments from the path.
  list($lng, $_, $_) = qcl::get_args_from_path();

  //$block['subject'] = t('Export');
  $block['content'] = drupal_get_form('qtrTranslations_export_form', $lng);
  return $block;
}

/**
 * Form callback for: qtrTranslations_export_form
 */
function qtrTranslations_export_form($form, &$form_state, $lng) {
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

  $form = [
    '#prefix' => '<div class="qcl-ui-filter clear-block">',
    '#suffix' => '</div>',

    'export' => [
      '#type' => 'fieldset',
      '#title' => t('Export'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,

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

      'export_mode' => [
        '#type' => 'select',
        '#title' => t('Export Mode'),
        '#description' => t('The export mode to be used.'),
        '#options' => [
          'most_liked' => t('The most liked translations'),
          'liked_by_me' => t('Translations liked by me'),
          'liked_by_users' => t('Translations liked by users'),
        ],
        '#default_value' => 'most_liked',
      ],

      'submit' => [
        '#value' => t('Export'),
        '#type' => 'submit',
      ],

      'users' => [
        '#type' => 'textarea',
        '#title' => t('List of Users'),
        '#description' => t('Enter usernames separated by comma.'),
        '#states' => [
          'visible' => [
            ':input[name="export_mode"]' => ['value' => 'liked_by_users'],
          ],
        ],
      ],
    ],
  ];

  if ($lng) unset($form['export']['lng']);

  return $form;
}

/**
 * Submit callback for the form: qtrTranslations_export_form
 */
function qtrTranslations_export_form_submit($form, &$form_state) {
  // If user is not authenticated, redirect to login first.
  if (!oauth2_user_is_authenticated()) {
    oauth2_user_authenticate($form_state, $redirection = ($form===NULL));
    return;
  }

  // Get lng from the path.
  list($lng, $_, $_) = qcl::get_args_from_path();

  $values = $form_state['values'];
  if (!$lng)  $lng = bcl::get_translation_lng();
  $chapter = check_plain($values['chapter']);
  $export_mode = check_plain($values['export_mode']);
  $users = check_plain($values['users']);

  if (in_array($export_mode, ['liked_by_me', 'liked_by_users'])) {
    $export_mode = 'preferance';
  }

  // Export the project.
  if (qcl::user_is_admin($lng)) {
    _qtrTranslations_export_now($lng, $chapter, $export_mode, $users);
  }
  else {
    _qtrTranslations_export_schedule($lng, $chapter, $export_mode, $users);
  }
}

/**
 * Export the project immediately.
 */
function _qtrTranslations_export_now($origin, $project, $lng, $export_mode, $friends) {
  $btr_server = variable_get('btrClient_server');
  drupal_goto($btr_server . '/project/export', [
      'absolute' => TRUE,
      'external' => TRUE,
      'query' => [
        'origin' => $origin,
        'project' => $project,
        'lng' => $lng,
        'export_mode' => $export_mode,
        'friends' => $friends,
      ],
    ]);
}

/**
 * Schedule a project export.
 */
function _qtrTranslations_export_schedule($origin, $project, $lng, $export_mode, $friends) {
  $btr = wsclient_service_load('btr');
  $result = $btr->export_projects([
              'origin' => $origin,
              'project' => $project,
              'lng' => $lng,
              'export_mode' => $export_mode,
              'preferred_likers' => $friends,
            ]);
  bcl::display_messages($result['messages']);
}
