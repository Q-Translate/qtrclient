<?php
/**
 * @file
 * Function: translateform_build()
 */

namespace BTranslator\Client;
use \qcl;

require_once __DIR__ . '/theme_functions.inc';

/**
 * List strings and the corresponding translations/suggestions.
 *
 * @param array $strings
 *   A multi-dimentional associative array of the string and the corresponding
 *   translations that are to be rendered.
 * @param string $lng
 *   The language code of the translations.
 *
 * @return array
 *   A render array of the given strings and translations.
 */
function translateform_build($strings, $lng) {
  // Include on the page the CSS/JS of the editor.
  qcl::translateform_include_editor();

  $pager = theme('pager', array('tags' => NULL, 'element' => 0));
  $form = array(
    'langcode' => array(
      '#type' => 'value',
      '#value' => $lng,
    ),
    'pager_top' => array(
      '#weight' => -10,
      '#markup' => $pager,
    ),
    'strings' => array(
      '#tree' => TRUE,
      '#theme' => 'qtrClient_translate_table',
      '#lng' => $lng,
    ),

    'buttons' => (count($strings) == 1 ?
               qcl::translateform_buttons($lng, key($strings)) :
               qcl::translateform_buttons($lng)),

    'pager_bottom' => array(
      '#weight' => 10,
      '#markup' => $pager,
    ),
  );

  // Fill in string values for the editor.
  foreach ($strings as $string) {
    $sguid = $string['sguid'];
    $form['strings'][$sguid] = qcl::translateform_string($string, $lng);
    // TODO: Display the number of comments for each string.
  }

  // If there is only one string, append social and discussions, etc.
  if (count($strings) == 1) {
    $form += qcl::translateform_meta($lng, $sguid, $string);
  }

  return $form;
}
