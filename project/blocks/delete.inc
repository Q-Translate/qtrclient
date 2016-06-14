<?php
/**
 * @file
 * Delete a project.
 */

/**
 * Called from hook_block_view for delete.
 */
function qtrProject_delete_view() {
  $block['subject'] = t('Delete Project');
  $block['content'] = drupal_get_form('qtrProject_delete_form');
  return $block;
}

/**
 * Form callback for: qtrProject_delete_form
 */
function qtrProject_delete_form($form, &$form_state) {
  // Suggest the username as the default value for the origin.
  $qtr_user = oauth2_user_get();
  $default_origin = str_replace(' ', '', $qtr_user['name']);
  if (!$default_origin) $default_origin = 'test';

  // Build the form.
  $form = [
    'delete' => [
      '#type' => 'fieldset',
      '#title' => t('Delete a project'),
      //'#description' => $description,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,

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

      'submit' => [
        '#value' => t('Delete'),
        '#type' => 'submit',
      ],
    ],
  ];
  return $form;
}

/**
 * Implement hook form_submit() for the form qtrProject_delete_form
 */
function qtrProject_delete_form_submit($form, &$form_state) {
  if (!oauth2_user_is_authenticated()) {
    oauth2_user_authenticate($form_state, $redirection = ($form===NULL));
    return;
  }

  $qtr = wsclient_service_load('qtr');
  $result = $qtr->delete_project([
              'origin' => $form_state['values']['origin'],
              'project' => $form_state['values']['project'],
            ]);
  qcl::display_messages([$result['message']]);
}
