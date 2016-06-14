<?php
/**
 * @file
 * Functions for the block statistics.
 */

/**
 * Called from hook_block_view for topcontrib.
 */
function qtrTranslations_statistics_view() {
  $lng = variable_get('qtrClient_translation_lng', 'fr');

  // Return cache if possible.
  $cid = "qtrTranslations_block_statistics:$lng";
  $cache = cache_get($cid, 'cache_qtrClient');
  if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
    return $cache->data;
  }

  $block['subject'] = t('Contribution statistics');
  $block['content'] = qcl::render_statistics($lng);

  // Cache for 12 hours.
  cache_set($cid, $block, 'cache_qtrClient', time() + 12 * 60 * 60);

  return $block;
}
