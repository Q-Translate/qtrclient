<?php
/**
 * @file
 * Functions for the block project_stats.
 */

/**
 * Called from hook_block_view for project_stats.
 */
function qtrProject_project_stats_view() {
  // Get the arguments from the path.
  list($origin, $project, $lng) = qcl::get_project_from_path();

  // Return cache if possible.
  $cid = "qtrProject_block_project_stats:$origin:$project:$lng";
  $cache = cache_get($cid, 'cache_qtrClient');
  if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
    return $cache->data;
  }

  // Build the block.
  $block['subject'] = t('Project statistics');
  $block['content'] = qcl::render_project_stats($origin, $project, $lng);

  // Cache for 12 hours.
  cache_set($cid, $block, 'cache_qtrClient', time() + 12 * 60 * 60);

  return $block;
}
