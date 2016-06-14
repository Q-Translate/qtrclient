<?php
/**
 * @file
 * Callback for menu item: 'qtr/project/list'
 */

/**
 * Menu callback: qtr/project/list
 */
function qtrProject_projlist() {
  $output = [
    'project_list' => [
      '#prefix' => '<div class="row">',
      '#suffix' => '</div>',

      'list' => [
        '#prefix' => '<div class="col-sm-6">',
        qcl::get_block('qtrProject', 'list'),
        '#suffix' => '</div>',
      ],

      'import' => [
        '#prefix' => '<div class="col-sm-6">',
        _qtrProject_projlist_import(),
        qcl::get_block('qtrProject', 'delete'),
        '#suffix' => '</div>',
      ],
    ],
  ];

  return $output;
}

/**
 * Import a project.
 */
function _qtrProject_projlist_import() {
  $import = qcl::get_block('qtrProject', 'import');
  $import['qtrProject_import']['import']['#collapsible'] = TRUE;
  $import['qtrProject_import']['import']['#collapsed'] = TRUE;
  return $import;
}
