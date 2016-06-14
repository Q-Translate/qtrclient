<?php
/**
 * @file
 * qcl_disqus.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function qcl_disqus_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
