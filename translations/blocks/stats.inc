<?php
/**
 * @file
 * Functions for the block stats.
 */

/**
 * Called from hook_block_view for topusers.
 */
function qtrTranslations_stats_view() {
  $lng = variable_get('qtrClient_translation_lng', 'en');

  // Return cache if possible.
  $cid = "qtrTranslations_block_stats:$lng";
  $cache = cache_get($cid, 'cache_qtrClient');
  if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
    return $cache->data;
  }

  $block['subject'] = t('Contribution stats');
  $block['content'] = qcl::render_stats($lng);

  // Cache for 12 hours.
  cache_set($cid, $block, 'cache_qtrClient', time() + 12 * 60 * 60);

  return $block;
}
