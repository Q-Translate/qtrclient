<?php
/**
 * @file
 * Menu callbacks.
 */

/**
 * Callback for 'qtr/rss-feed'.
 */
function qtrClient_rss_feed($lng = NULL) {
  // Get the translation language.
  if ($lng === NULL)  $lng = qcl::get_translation_lng();

  // Get the feed data from cache, if possible.
  $cache = cache_get("qtrClient_rss_feed:$lng", 'cache_qtrClient');
  if (!empty($cache) && isset($cache->data) && !empty($cache->data)) {
    $rss_feed = $cache->data;
    drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');
    print $rss_feed;
    return;
  }

  // Get the latest translations from the Q-Translate server.
  $qtr_server = variable_get('qtrClient_server', NULL);
  if (empty($qtr_server)) {
    $translations = array();
  }
  else {
    $url = $qtr_server . "/latest/$lng";
    $output = drupal_http_request($url);
    $translations = json_decode($output->data, TRUE);
  }

  // Start building the RSS feed output.
  $rss_feed = '<?xml version="1.0" encoding="utf-8"?>' . "\n";
  $rss_feed .= '<rss version="2.0">';

  $base_url = $GLOBALS['base_url'];
  $link = $base_url . "/qtr/rss-feed/$lng";
  $title = variable_get('site_name', 'Q-Translate');
  $description = t('Latest translations on Q-Translate');
  $ttl = 180;
  $site_mail = variable_get('site_mail');
  $site_name = variable_get('site_name');
  $web_master = "$site_mail ($site_name)";

  $last_build_date = date('r');
  $rss_feed .= "
    <channel>
      <title>$title</title>
      <link>$link</link>
      <description>$description</description>
      <category>Q-Translate</category>
      <language>$lng</language>
      <ttl>$ttl</ttl>
      <webMaster>$web_master</webMaster>
      <lastBuildDate>$last_build_date</lastBuildDate>
    ";

  // Append an item for each translation.
  foreach ($translations as $trans) {
    extract($trans);
    $title = qcl::shorten($translation, 60);
    $link = $base_url . "/qtr/$lng/$chapter_id/$verse_nr";
    $description = "[$chapter_id:$verse_nr] $translation";
    $pub_date = date('r', strtotime($time));
    $rss_feed .= "
      <item>
        <title>$title</title>
        <link>$link</link>
        <description><![CDATA[$description]]></description>
        <author>$usermail ($username)</author>
        <guid isPermaLink='false'>$tguid</guid>
        <pubDate>$pub_date</pubDate>
      </item>
    ";
  }
  // Close the XML elements.
  $rss_feed .= "</channel>\n";
  $rss_feed .= "</rss>\n";

  // Cache for 2 hours.
  cache_set("qtrClient_rss_feed:$lng", $rss_feed, 'cache_qtrClient', strtotime('+2 hours'));

  // Send the output.
  drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');
  print $rss_feed;
}
