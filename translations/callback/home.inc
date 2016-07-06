<?php
/**
 * @file
 * Functions for the menu path: 'qtr/'
 */

/**
 * Return the homepage or dashboard for translations.
 */
function qtrTranslations_home() {
  $output[] = qcl::get_block('qtrTranslations', 'select');
  $output[] = array(
    'stats' => [
      '#prefix' => '<div class="row">',
      '#suffix' => '</div>',

      'stats' => [
        '#prefix' => '<div class="col-sm-6">',
        qcl::get_block('qtrTranslations', 'stats'),
        '#suffix' => '</div>',
      ],

      'topusers' => [
        '#prefix' => '<div class="col-sm-6">',
        qcl::get_block('qtrTranslations', 'topusers'),
        '#suffix' => '</div>',
      ],
    ],
  );

  return $output;
}
