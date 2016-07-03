<?php
/**
 * @file
 * Function: translateform_include_editor()
 */

namespace QTranslate\Client;
use \qcl;

/**
 * Include the resources of the editor (CSS, JS, etc.)
 */
function translateform_include_editor() {
  // Add the CSS and JS files.
  $path = drupal_get_path('module', 'qtrClient') . '/editor/';
  drupal_add_css($path . 'editor.css');
  drupal_add_js($path . 'jquery.worddiff.js');
  drupal_add_js($path . 'editor.js');

  // Add RTL style if the current language's direction is RTL.
  $languages = qcl::get_languages();
  if ($languages[$lng]['direction'] == LANGUAGE_RTL) {
    drupal_add_css($path . 'editor-rtl.css');
  }
}
