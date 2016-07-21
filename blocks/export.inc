<?php
/**
 * @file
 * Functions for the block export.
 */

/**
 * Called from hook_block_view for export.
 */
function qtrClient_export_view() {
  // Get the arguments from the path.
  list($lng, $_, $_) = qcl::get_args_from_path();

  //$block['subject'] = t('Export');
  $block['content'] = drupal_get_form('qtrClient_export_form', $lng);
  return $block;
}

/**
 * Form callback for: qtrClient_export_form
 */
function qtrClient_export_form($form, &$form_state, $lng) {
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

      'mode' => [
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
            ':input[name="mode"]' => ['value' => 'liked_by_users'],
          ],
        ],
      ],
    ],
  ];

  return $form;
}

/**
 * Submit callback for the form: qtrClient_export_form
 */
function qtrClient_export_form_submit($form, &$form_state) {
  // Get the other parameters.
  $values = $form_state['values'];
  $lng = check_plain($values['lng']);
  $chapter = check_plain($values['chapter']);
  $mode = check_plain($values['mode']);
  $users = check_plain($values['users']);

  if ($mode == 'liked_by_me' || $mode == 'liked_by_users') {
    $mode = 'preference';
  }

  // Export
  $qtr_server = variable_get('qtrClient_server');
  drupal_goto($qtr_server . '/export', [
      'absolute' => TRUE,
      'external' => TRUE,
      'query' => [
        'lng' => $lng,
        'chapter' => $chapter,
        'mode' => $mode,
        'users' => $users,
      ],
    ]);
}
