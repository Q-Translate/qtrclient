<?php
/**
 * @file
 * Callback for menu item: 'qtr/project/%/%/%/dashboard'
 */

/**
 * Menu callback for 'qtr/project/%/%/%/dashboard'
 */
function qtrProject_dashboard($origin, $project, $lng) {
  $output[] = qcl::get_block('qtrProject', 'select');
  $output[] = qcl::get_block('qtrProject', 'subscribe');
  $output[] = [ '#markup' => '<hr style="clear: both;"/>' ];
  $output[] = _qtrProject_dashboard_stats();
  $output[] = [ '#markup' => '<hr style="clear: both;"/>' ];
  $output[] = _qtrProject_export_import_diffs();

  return $output;
}

/**
 * Render-array for stats.
 */
function _qtrProject_dashboard_stats() {
  return [
    'stats' => [
      '#prefix' => '<div class="row">',
      '#suffix' => '</div>',

      'statistics' => [
        '#prefix' => '<div class="col-sm-4">',
        qcl::get_block('qtrProject', 'statistics'),
        '#suffix' => '</div>',
      ],

      'topcontrib' => [
        '#prefix' => '<div class="col-sm-4">',
        qcl::get_block('qtrProject', 'topcontrib'),
        '#suffix' => '</div>',
      ],

      'project_stats' => [
        '#prefix' => '<div class="col-sm-4">',
        qcl::get_block('qtrProject', 'project_stats'),
        '#suffix' => '</div>',
      ],
    ],
  ];
}

/**
 * Render-array for export-import-diffs.
 */
function _qtrProject_export_import_diffs() {
  $export = qcl::get_block('qtrProject', 'export');
  $export['qtrProject_export']['export']['#collapsible'] = TRUE;
  $export['qtrProject_export']['export']['#collapsed'] = TRUE;

  $import = qcl::get_block('qtrProject', 'import_translations');
  $import['qtrProject_import_translations']['import_translations']['#collapsible'] = TRUE;
  $import['qtrProject_import_translations']['import_translations']['#collapsed'] = TRUE;

  $diffs = qcl::get_block('qtrProject', 'diffs');
  $diffs['qtrProject_diffs']['project_diffs']['#collapsible'] = TRUE;
  $diffs['qtrProject_diffs']['project_diffs']['#collapsed'] = TRUE;

  return [
    'export_import' => [
      '#prefix' => '<div class="row">',
      '#suffix' => '</div>',

      'export' => [
        '#prefix' => '<div class="col-sm-6">',
        $export,
        '#suffix' => '</div>',
      ],

      'import' => [
        '#prefix' => '<div class="col-sm-6">',
        $import,
        '#suffix' => '</div>',
      ],

      'diffs' => [
        '#prefix' => '<div class="col-sm-12">',
        $diffs,
        '#suffix' => '</div>',
      ],
    ],
  ];
}
