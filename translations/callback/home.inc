<?php
/**
 * @file
 * Functions for the menu path: 'qtr/'
 */

/**
 * Return the homepage or dashboard for translations.
 */
function qtrTranslations_home() {
  $block_select = qcl::get_block('qtrTranslations', 'select');
  $block_select['qtrTranslations_select']['#id'] = 'qtrtranslations-select-form-vertical';

  $output[] = array(
    'first-row' => [
      '#prefix' => '<div class="row">',
      '#suffix' => '</div>',

      'select-translations' => [
        '#prefix' => '<div class="col-sm-4">',
        $block_select,
        '#suffix' => '</div>',
      ],
      'stats' => [
        '#prefix' => '<div class="col-sm-4">',
        qcl::get_block('qtrTranslations', 'stats'),
        '#suffix' => '</div>',
      ],

      'topusers' => [
        '#prefix' => '<div class="col-sm-4">',
        qcl::get_block('qtrTranslations', 'topusers'),
        '#suffix' => '</div>',
      ],
    ],
  );

  return $output;
}
