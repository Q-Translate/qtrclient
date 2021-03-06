<?php
/**
 * @file
 * Functions for the block stats.
 */

/**
 * Called from hook_block_view for topusers.
 */
function qtrClient_stats_view() {
  list($lng, $_, $_) = qcl::get_args_from_path();
  if (!$lng) $lng = variable_get('qtrClient_translation_lng', 'en');

  // Return cache if possible.
  $cid = "qtrClient_block_stats:$lng";
  $cache = cache_get($cid, 'cache_qtrClient');
  if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
    return $cache->data;
  }

  $block['subject'] = t('Translation stats');
  $block['content'] = qcl::render_stats($lng);

  // Cache for 12 hours.
  cache_set($cid, $block, 'cache_qtrClient', time() + 12 * 60 * 60);

  return $block;
}
