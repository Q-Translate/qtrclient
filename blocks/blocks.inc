<?php
/**
 * @file
 * Defining blocks of qtrClient.
 */

require_once __DIR__ . '/navigate.inc';
require_once __DIR__ . '/select.inc';
require_once __DIR__ . '/search.inc';
require_once __DIR__ . '/topusers.inc';
require_once __DIR__ . '/stats.inc';
require_once __DIR__ . '/latest.inc';
require_once __DIR__ . '/export.inc';
require_once __DIR__ . '/import.inc';

/**
 * Implements hook_block_info().
 *
 * This hook declares what blocks are provided by the module.
 */
function qtrClient_block_info() {
  // This hook returns an array, each component of which is an array of block
  // information. The array keys are the 'delta' values used in other block
  // hooks.

  // The required block information is a block description, which is shown
  // to the site administrator in the list of possible blocks. You can also
  // provide initial settings for block weight, status, etc.

  // Subscribe to a project.
  $blocks['navigate'] = array(
    'info' => t('Q-Translate: Go to the next/prev verse, etc.'),
    'cache' => DRUPAL_NO_CACHE,
  );

  // Select a verse/language translations.
  $blocks['select'] = array(
    'info' => t('Q-Translate: Select verse/language translations.'),
    'cache' => DRUPAL_CACHE_CUSTOM,
  );

  // Search translations.
  $blocks['search'] = array(
    'info' => t('Q-Translate: Search translations.'),
    'cache' => DRUPAL_CACHE_CUSTOM,
  );

  // Displays top 5 (or 10) contributors, over the last week (or month).
  $blocks['topusers'] = array(
    'info' => t('Q-Translate: Top contributors during the last period'),
    'cache' => DRUPAL_CACHE_CUSTOM,
  );

  // Suggestion and voting stats for the last week, month and year.
  $blocks['stats'] = array(
    'info' => t('Q-Translate: Contribution statisticss (for the last week, month and year)'),
    'cache' => DRUPAL_CACHE_CUSTOM,
  );

  // Suggestions and likes submited lately.
  $blocks['latest'] = array(
    'info' => t('Q-Translate: Latest contributions'),
    'cache' => DRUPAL_CACHE_CUSTOM,
  );

  // Export the project.
  $blocks['export'] = array(
    'info' => t('Q-Translate: Export'),
    'cache' => DRUPAL_NO_CACHE,
  );

  // Import.
  $blocks['import'] = array(
    'info' => t('Q-Translate: Import'),
    'cache' => DRUPAL_NO_CACHE,
  );

  return $blocks;
}

/**
 * Implements hook_block_configure().
 *
 * This hook declares configuration options for blocks provided by this module.
 */
function qtrClient_block_configure($delta = '') {
  $form = array();
  // The $delta parameter tells us which block is being configured.
  $config = 'qtrClient_' . $delta . '_config';
  if (function_exists($config)) {
    $form = $config();
  }
  return $form;
}


/**
 * Implements hook_block_save().
 *
 * This hook declares how the configured options for a block
 * provided by this module are saved.
 */
function qtrClient_block_save($delta = '', $edit = array()) {
  $save = 'qtrClient_' . $delta . '_save';
  if (function_exists($save))  $save($edit);
}

/**
 * Implements hook_block_view().
 *
 * This hook generates the contents of the blocks themselves.
 */
function qtrClient_block_view($delta = '') {
  // The $delta parameter tells us which block is being requested.
  $view = 'qtrClient_' . $delta . '_view';
  if (function_exists($view)) {
    $block = $view();
  }
  return $block;
}
