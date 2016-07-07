<?php
/**
 * @file
 * Functions for the block topusers.
 */

/**
 * Called from hook_block_config for topusers.
 *
 * All we need to provide is the specific configuration options for our
 * block. Drupal will take care of the standard block configuration options
 * (block title, page visibility, etc.) and the save button.
 */
function qtrTranslations_topusers_config() {
  $form['qtrTranslations_topusers_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Size of the list'),
    '#description' => t('The number of top contributers that will be shown in the list (5, 10, etc)'),
    '#default_value' => variable_get('qtrTranslations_topusers_size', 5),
  );
  $form['qtrTranslations_topusers_period'] = array(
    '#type' => 'select',
    '#title' => t('Accumulation period'),
    '#description' => t('The period during which the contributions will be counted (last week or last month).'),
    '#options' => array(
      'week' => t('Last week'),
      'month' => t('Last month'),
    ),
    '#default_value' => variable_get('qtrTranslations_topusers_period', 'week'),
  );
  return $form;
}

/**
 * Called from hook_block_save for topusers.
 */
function qtrTranslations_topusers_save($edit) {
  // Have Drupal save them to the database.
  variable_set('qtrTranslations_topusers_size', $edit['qtrTranslations_topusers_size']);
  variable_set('qtrTranslations_topusers_period', $edit['qtrTranslations_topusers_period']);
}

/**
 * Called from hook_block_view for topusers.
 */
function qtrTranslations_topusers_view() {
  // Get parameters.
  $period = variable_get('qtrTranslations_topusers_period', 'week');
  $size = variable_get('qtrTranslations_topusers_size', 5);
  list($lng, $_, $_) = qcl::get_args_from_path();
  if (!$lng) $lng = variable_get('qtrClient_translation_lng', 'en');

  // Return cache if possible.
  $cid = "qtrTranslations_block_topusers:$period:$size:$lng";
  $cache = cache_get($cid, 'cache_qtrClient');
  if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
    return $cache->data;
  }

  // Build the block view.
  $block['subject'] = t('Top @nr translators', array('@nr' => $size));
  $block['content'] = qcl::render_topusers($period, $size, $lng);

  // Cache for 12 hours.
  cache_set($cid, $block, 'cache_qtrClient', time() + 12 * 60 * 60);

  return $block;
}
