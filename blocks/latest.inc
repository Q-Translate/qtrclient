<?php
/**
 * @file
 * Functions for the block latest.
 */

/**
 * Called from hook_block_view for latest.
 */
function qtrClient_latest_view() {
  list($lng, $_, $_) = qcl::get_args_from_path();
  if (!$lng) $lng = variable_get('qtrClient_translation_lng', 'en');

  // Return cache if possible.
  $cache_id = "qtrClient_block_latest:$lng";
  $cache = cache_get($cache_id, 'cache_qtrClient');
  if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
    return $cache->data;
  }

  // Get the latest 5 translations from the Q-Translate server.
  $qtr_server = variable_get('qtrClient_server', NULL);
  if (empty($qtr_server)) {
    $translations = array();
  }
  else {
    $url = $qtr_server . "/latest/$lng//5";
    $output = qcl::http_request($url);
    $translations = json_decode($output->data, TRUE);
    if (!is_array($translations)) $translations = array();
  }

  // Generate the content of the block.
  $content = '<ul>';
  foreach ($translations as $trans) {
    extract($trans);
    $url = $base_url . "/qtr/$lng/$chapter_id/$verse_nr";

    $author = $username;
    if ($time) {
      $time = format_interval(REQUEST_TIME - strtotime($time));
      $author .= ", $time ago";
    }

    $content .= "<li><a href='$url' target='_blank'"
             . " class='btn btn-default btn-sm' style='padding:0px 3px;'>"
             . " $chapter_id:$verse_nr </a> "
             . qcl::shorten($translation, 100)
             . " <span style='font-size:x-small; color:gray;'> -- $author</span>"
             . "</li>\n";
  }
  $content .= '</ul>';

  // Set the subject and content of the block.
  $block['subject'] = t('Latest translations');
  $block['content'] = $content;
  if ($content == '<ul></ul>')  return $block;

  // Cache for 2 hours.
  cache_set($cache_id, $block, 'cache_qtrClient', time() + 2 * 60 * 60);

  return $block;
}
