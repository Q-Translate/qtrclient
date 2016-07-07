<?php
/**
 * @file
 * Function: translateform_build()
 */

namespace QTranslate\Client;
use \qcl;

require_once __DIR__ . '/theme_functions.inc';

/**
 * List verses and the corresponding translations.
 *
 * @param array $verses
 *   A multi-dimentional associative array of the verse and the corresponding
 *   translations that are to be rendered.
 * @param verse $lng
 *   The language code of the translations.
 *
 * @return array
 *   A render array of the given verses and translations.
 */
function translateform_build($verses, $lng) {
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
    'verses' => array(
      '#tree' => TRUE,
      '#theme' => 'qtrClient_translate_table',
      '#lng' => $lng,
    ),

    'buttons' => (count($verses) == 1 ?
               qcl::translateform_buttons($lng, $verses[0]) :
               qcl::translateform_buttons($lng)),

    'pager_bottom' => array(
      '#weight' => 10,
      '#markup' => $pager,
    ),
  );

  // Fill in verse values for the editor.
  foreach ($verses as $verse) {
    $vid = $verse['vid'];
    $form['verses'][$vid] = qcl::translateform_verse($verse, $lng);
    // TODO: Display the number of comments for each verse.
  }

  // If there is only one verse, append social and discussions, etc.
  if (count($verses) == 1) {
    $form += qcl::translateform_meta($lng, $verse);
  }

  return $form;
}
