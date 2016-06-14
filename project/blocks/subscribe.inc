<?php
/**
 * @file
 * Subscribe to a project.
 */

/**
 * Called from hook_block_view for subscribe.
 */
function qtrProject_subscribe_view() {
  // Get the arguments from the path.
  list($origin, $project, $lng) = qcl::get_project_from_path();

  $block['subject'] = t('Subscribe to Project');
  $block['content'] = drupal_get_form('qtrProject_subscribe_form', $origin, $project);
  return $block;
}

/**
 * Form callback for: qtrProject_subscribe_form
 */
function qtrProject_subscribe_form($form, &$form_state, $origin, $project) {
  $subscribed = _qtrProject_user_subscribed($origin, $project);
  $form = [
    'origin' => [
      '#type' => 'hidden',
      '#value' => $origin,
    ],
    'project' => [
      '#type' => 'hidden',
      '#value' => $project,
    ],
    'subscribe' => [
      '#value' => '<span class="glyphicon glyphicon-envelope"></span> - ' . t('Subscribe'),
      '#type' => 'submit',
      '#attributes' => ['class' => ['btn-primary']],
      '#access' => !$subscribed,
    ],
    'unsubscribe' => [
      '#value' => '<span class="glyphicon glyphicon-envelope"></span> - ' . t('Unsubscribe'),
      '#type' => 'submit',
      '#attributes' => ['class' => ['btn-primary']],
      '#access' => $subscribed,
    ],
  ];

  return $form;
}

/**
 * Return TRUE if the user is subscribed to the given project.
 */
function _qtrProject_user_subscribed($origin, $project) {
  if (!oauth2_user_is_authenticated()) return FALSE;

  // Check the given project on the list of projects.
  $qtr_user = oauth2_user_get();
  if (!isset($qtr_user['subscribed_projects']))  return FALSE;
  return in_array("$origin/$project", $qtr_user['subscribed_projects']);
}

/**
 * Submit callback for the form: qtrProject_subscribe_form
 */
function qtrProject_subscribe_form_submit($form, &$form_state) {
  // If user is not authenticated, redirect to login first.
  if (!oauth2_user_is_authenticated()) {
    oauth2_user_authenticate($form_state, $redirection = ($form===NULL));
    return;
  }

  // Get the parameters.
  $values = $form_state['values'];
  $operation = $values['op'];
  $origin = $values['origin'];
  $project = $values['project'];

  $qtr = wsclient_service_load('qtr');
  if ($operation == '<span class="glyphicon glyphicon-envelope"></span> - ' . t('Subscribe')) {
    // Subscribe to the project.
    $qtr->subscribe(['origin' => $origin, 'project' => $project]);
    $msg = t("Subscribed to project '!project'. You will receive daily emails from this project.",
           ['!project' => "$origin/$project"]);
  }
  elseif ($operation == '<span class="glyphicon glyphicon-envelope"></span> - ' . t('Unsubscribe')) {
    // Unsubscribe from the project.
    $qtr->unsubscribe(['origin' => $origin, 'project' => $project]);
    $msg = t("Unsubscribed from project '!project'. You will not receive daily emails from this project.",
           ['!project' => "$origin/$project"]);
  }
  drupal_set_message($msg);

  // Expire from cache the qtr_user, so that it can be
  // refreshed with the latest values.
  oauth2_user_expire();
}
