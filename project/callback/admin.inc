<?php
/**
 * @file
 * Callback for menu item: 'qtr/project/%/%/%/admin'
 */

/**
 * Menu callback for 'qtr/project/%/%/%/admin'
 */
function qtrProject_admin($origin, $project, $lng) {
  $output[] = drupal_get_form('qtrProject_admin_sync', $origin, $project, $lng);
  return $output;
}

/**
 * Form callback for: qtrProject_admin_sync
 */
function qtrProject_admin_sync($form, &$form_state, $origin, $project, $lng) {
  $qtr_server = variable_get('qtrClient_server', 'https://qtranslate.org');
  $form['sync'] = array(
    '#access' =>  qcl::user_is_project_admin($origin, $project, $lng),
    '#markup' => l(t('Synchronize'), "$qtr_server/project/sync",
               [
                 'external' => TRUE,
                 'attributes' => [
                   'class' => ['btn', 'btn-default', 'form-submit'],
                   'target' => '_blank',
                 ],
                 'query' => [
                   'origin' => $origin,
                   'project' => $project,
                   'lng' => $lng,
                 ],
               ]),
  );

  return $form;
}
