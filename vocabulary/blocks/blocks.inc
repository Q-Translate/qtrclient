<?php
/**
 * @file
 * Defining blocks of qtrVocabulary.
 */

require_once __DIR__ . '/list.inc';
require_once __DIR__ . '/import.inc';
require_once __DIR__ . '/delete.inc';

/**
 * Implements hook_block_info().
 *
 * This hook declares what blocks are provided by the module.
 */
function qtrVocabulary_block_info() {
  // This hook returns an array, each component of which is an array of block
  // information. The array keys are the 'delta' values used in other block
  // hooks.

  // The required block information is a block description, which is shown
  // to the site administrator in the list of possible blocks. You can also
  // provide initial settings for block weight, status, etc.

  // List of vocabularies.
  $blocks['list'] = array(
    'info' => t('Q-Translate Vocabularies: List of vocabularies'),
    'cache' => DRUPAL_NO_CACHE,
  );

  // Import.
  $blocks['import'] = array(
    'info' => t('Q-Translate Vocabulary: Import'),
    'cache' => DRUPAL_NO_CACHE,
  );

  // Delete.
  $blocks['delete'] = array(
    'info' => t('Q-Translate Vocabulary: Delete'),
    'cache' => DRUPAL_NO_CACHE,
  );

  return $blocks;
}

/**
 * Implements hook_block_configure().
 *
 * This hook declares configuration options for blocks provided by this module.
 */
function qtrVocabulary_block_configure($delta = '') {
  $form = array();
  // The $delta parameter tells us which block is being configured.
  $config = 'qtrVocabulary_' . $delta . '_config';
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
function qtrVocabulary_block_save($delta = '', $edit = array()) {
  $save = 'qtrVocabulary_' . $delta . '_save';
  if (function_exists($save))  $save($edit);
}

/**
 * Implements hook_block_view().
 *
 * This hook generates the contents of the blocks themselves.
 */
function qtrVocabulary_block_view($delta = '') {
  // The $delta parameter tells us which block is being requested.
  $view = 'qtrVocabulary_' . $delta . '_view';
  if (function_exists($view)) {
    $block = $view();
  }
  return $block;
}
